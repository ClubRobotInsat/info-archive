#include "environment.h"
#include "ConstantesCommunes.h"
#include "TGAWriter.h"

#include <Json.h>
#include <fstream>

using namespace std;

const float Environment::DANGER_INFINITY = 2;

const repere::Repere REFERENCE_ENVIRONMENT =
    repere::Repere({0_m, 0_m}, repere::Multiplicateur::SENS_POSITIF, repere::Multiplicateur::SENS_POSITIF);

Neighbor::Neighbor(Distance distance, Node& n_) : dist(distance), n(n_) {}

Nodemap const& Environment::getNodes() const {
	return _nodes;
}

Environment::Environment(Vector2u16 size, Distance scale, Distance robotWidth, Distance robotRadius, Vector2m offset) {
	_size = size;
	_scale = scale;
	_offset = offset;
	initNodes();

	_robotWidth = robotWidth / 2; // on récupère la demi longueur !
	_radius = robotRadius;

	// Initialisation des paramètres par défaut :
	_postProcessParameters.linearizationEnabled = true;
	_postProcessParameters.angularLinearization.enabled = true;
	_postProcessParameters.angularLinearization.discardTreshold = 4_cm;
	_postProcessParameters.angularLinearization.threshold = 1.5_deg;
	_postProcessParameters.bezier.enabled = true;
	_postProcessParameters.bezier.dangerThreshold = DANGER_INFINITY / 2;
	_postProcessParameters.bezier.pointsCount = 3;
	_postProcessParameters.bezier.angleFilterEnabled = true;
	_postProcessParameters.bezier.minAngle = 5_deg;
	_postProcessParameters.trajectorySegmentation.enabled = true;
	_postProcessParameters.trajectorySegmentation.maxSpacing = 20_cm;
}

void Environment::initNodes() {
	_nodes.clear();
	for(int x = 0; x < _size.x; x++) {
		std::vector<Node> v;
		for(int y = 0; y < _size.y; y++) {
			v.emplace_back(x, y, 0, 0, 0);
		}
		_nodes.push_back(std::move(v));
	}
}

struct NodeComp {
	bool operator()(Node* n1, Node* n2) const {
		return *n1 < *n2;
	}
};


vector<Vector2m> Environment::getTrajectory(Vector2m start, Vector2m end, Duration timeout, bool cubic_interpolation, int /*maxSteps*/) {
	// On ajoute l'offset
	start += this->_offset;
	end += this->_offset;

	// On remet les noeuds dans leur état initial.
	resetNodes(_nodes);

	// On dessine les formes si elles ont changé.
	if(_staticDirty)
		drawStaticShapes(_nodes);
	if(_dynamicDirty)
		drawDynamicShapes(_nodes);

	_dynamicDirty = false;
	_staticDirty = false;


	// Gère le cas où le robot se trouve dans une zone "interdite" : il faut qu'il puisse en sortir.
	if(this->getDangerValueSafeAt(start) >= DANGER_INFINITY) {
		auto oldStart = start;
		try {
			std::cout << "Départ invalide, getEscapePosition()" << std::endl;
			start = this->getEscapePosition(start, timeout);
		} catch(std::runtime_error&) {
			std::cout << "Impossible de trouver une position de fuite à partir de " << start << " !" << std::endl;
		}
		cout << "[Environment:getTrajectory] Position de départ (" << oldStart << ") invalide => modifiée (" << start
		     << ") !" << endl;
	}

	// Gère le cas où l'arrivée se trouve dans une zone interdite :
	// on se rapproche le + possible de l'endroit demandé
	if(this->getDangerValueSafeAt(end) >= DANGER_INFINITY) {
		auto oldEnd = end;
		try {
			std::cout << "Arrivée invalide, getEscapePosition()" << std::endl;
			end = this->getEscapePosition(end, timeout);
		} catch(std::runtime_error&) {
			end = start;
			std::cout << "Impossible de trouver une position de fuite à partir de " << end << " !" << std::endl;
		}
		cout << "[Environment:getTrajectory] Position d'arrivée (" << oldEnd << ") invalide => modifiée (" << end << ") !" << endl;
	}

	// Position
	int ex = toGridUnit(end.x);
	int ey = toGridUnit(end.y);

	// Fin
	int sx = toGridUnit(start.x);
	int sy = toGridUnit(start.y);


	// Vérification des bornes.
	if(sx < 0 || sy < 0 || sx >= _size.x || sy >= _size.y) {
		cout << "Point de départ invalide" << endl;
		throw runtime_error("Point de départ invalide.");
	}
	if(ex < 0 || ey < 0 || ex >= _size.x || ey >= _size.y) {
		cout << "Point d'arrivée invalide" << endl;
		throw runtime_error("Point d'arrivée invalide.");
	}


	// Cas où départ = arrivée
	if(ex == sx && ey == sy) {
		vector<Vector2m> trajectory;
		trajectory.push_back(start);
		trajectory.push_back(end);
		return trajectory;
	}

	set<Node*, NodeComp> candidateNodes;
	unordered_set<Node*> doneNodes;
	unordered_map<Node*, Node*> previousNodes;


	// Initialisation
	_nodes[sx][sy].heuristicCost = getHeuristicCost(_nodes[sx][sy], _nodes[ex][ey]);
	candidateNodes.insert(&_nodes[sx][sy]);

	while(candidateNodes.size() != 0) {
		// Traite le noeud en cours.
		auto candidateIt = candidateNodes.begin();
		Node* candidate = *(candidateIt);

		if(candidate->x == ex && candidate->y == ey) {
			return postProcess(getPathTo(_nodes[sx][sy], *candidate, previousNodes), cubic_interpolation);
		}

		candidateNodes.erase(candidateIt);
		doneNodes.insert(candidate);
		candidate->_debugValue = 125;

		// Ajoute les voisins à la liste.
		vector<Neighbor> neighbors = getNeighbors(*candidate);
		for(auto it = neighbors.begin(); it != neighbors.end(); it++) {

			Node& neighbor = it->n;
			bool contains = candidateNodes.find(&neighbor) != candidateNodes.end();
			contains |= doneNodes.count(&neighbor) > 0;


			// Empeche l'algo d'aller dans les zones de danger infinies.
			if(neighbor.staticValue >= DANGER_INFINITY)
				continue;
			if(neighbor.dynamicValue >= DANGER_INFINITY)
				continue;

			// Coût du noeud actuel au voisin
			float cost = neighbor.staticValue + neighbor.dynamicValue;
			cost = std::max(cost, 1.0f) * it->dist.toM();
			// Coût effectif du départ à ce noeud.
			float effectiveCost = candidate->effectiveCost + cost;


			if(!contains || effectiveCost < neighbor.effectiveCost) {

				// Coût heuristique du départ au noeud final en passant par ce noeud.
				float heuristicCost = effectiveCost + getHeuristicCost(neighbor, _nodes[ex][ey]);
				previousNodes[&neighbor] = candidate;

				if(contains) {
					candidateNodes.erase(&neighbor);
				}

				neighbor.effectiveCost = effectiveCost;
				neighbor.heuristicCost = heuristicCost;

				candidateNodes.insert(&neighbor);
			}
		}
	}
	vector<Vector2m> defaultValue;
	return defaultValue;
}


float Environment::getHeuristicCost(Node& from, Node& to) {
	auto sx = toWorldUnit(from.x - to.x);
	auto sy = toWorldUnit(from.y - to.y);
	auto val = sqrt(sx * sx + sy * sy);
	return val.toM();
}

bool Environment::isForbidden(Vector2m const& position) const {
	return this->getDangerValueSafeAt(position) >= DANGER_INFINITY * 0.92;
}

bool Environment::loadFromFile(std::string filename) {
	bool read;

	JSON json;
	std::ifstream in(filename);

	if(in >> json) {
		try {
			loadFromJSON(json);
			read = true;
		} catch(...) {
			read = false;
		}
	} else {
		read = false;
	}
	in.close();

	return read;
}

void Environment::loadFromJSON(JSON json) {
	const float danger{Environment::DANGER_INFINITY};

	const JSON& objects = json["objects"];

	for(const JSON& object : objects) {
		if(object["A*"]["enabled"].get<bool>()) {
			std::string type(object["type"].get<std::string>());

			Vector2m position(Json::toVector2m(object["position"]));
			Angle angle(Angle::makeFromDeg(object["angle"].get<double>()));

			if(type == "cuboid") {
				Vector2m dimensions(Json::toVector2m(object["dimensions"]));
				Vector2m offset_position{Distance::makeFromMm(position.x.toMm() - dimensions.x.toMm() * cos(angle.toRad()) / 2 +
				                                              dimensions.y.toMm() * sin(angle.toRad()) / 2),
				                         Distance::makeFromMm(position.y.toMm() + dimensions.x.toMm() * sin(angle.toRad()) / 2 -
				                                              dimensions.y.toMm() * cos(angle.toRad()) / 2)};
				repere::Coordonnees coords(offset_position, angle);
				this->addStaticShape(std::make_unique<Rect>(
				    danger, coords.getPos2D(REFERENCE_ENVIRONMENT), dimensions, coords.getAngle(REFERENCE_ENVIRONMENT)));
			} else if(type == "cylinder" || type == "sphere") {
				Length radius(Length::makeFromM(object["radius"].get<double>()));
				repere::Coordonnees coords(position);
				this->addStaticShape(std::make_unique<Circle>(danger, radius, coords.getPos2D(REFERENCE_ENVIRONMENT)));
			}
		}
	}
}

void Environment::addNeighbor(vector<Neighbor>& neighbors, int x, int y, Distance distance) {
	if(x < 0 || y < 0 || x >= _size.x || y >= _size.y)
		return;
	neighbors.push_back(Neighbor(distance, _nodes[x][y]));
}

vector<Neighbor> Environment::getNeighbors(Node& n) {
	const float sqrt2 = 1.41f;
	vector<Neighbor> neighbors;
	bool left = n.x > 0;
	bool up = n.y > 0;
	bool right = n.x < _nodes.size() - 1;
	bool down = n.y < _nodes[0].size() - 1;
	if(left)
		neighbors.emplace_back(toWorldUnit(1), _nodes[n.x - 1][n.y]);
	if(up)
		neighbors.emplace_back(toWorldUnit(1), _nodes[n.x][n.y - 1]);
	if(right)
		neighbors.emplace_back(toWorldUnit(1), _nodes[n.x + 1][n.y]);
	if(down)
		neighbors.emplace_back(toWorldUnit(1), _nodes[n.x][n.y + 1]);

	if(left && up)
		neighbors.emplace_back(toWorldUnit(sqrt2), _nodes[n.x - 1][n.y - 1]);
	if(left && down)
		neighbors.emplace_back(toWorldUnit(sqrt2), _nodes[n.x - 1][n.y + 1]);
	if(right && up)
		neighbors.emplace_back(toWorldUnit(sqrt2), _nodes[n.x + 1][n.y - 1]);
	if(right && down)
		neighbors.emplace_back(toWorldUnit(sqrt2), _nodes[n.x + 1][n.y + 1]);
	return neighbors;
}

vector<Vector2m> Environment::getPathTo(Node& from, Node& to, unordered_map<Node*, Node*>& previousNodes) {
	vector<Vector2m> pts;
	if(to == from) {
		pts.push_back(Vector2m(toWorldUnit(to.x), toWorldUnit(to.y)));
		return pts;
	}

	Node* current = &to;
	while(!(*current == from)) {
		pts.push_back(Vector2m(toWorldUnit(current->x), toWorldUnit(current->y)));
		current = previousNodes[current];
	}
	return pts;
}

Vector2m Environment::getEscapePosition(Vector2m start, Duration timeout) {
	int size = 1;
	int gridX = toGridUnit(start.x);
	int gridY = toGridUnit(start.y);
	auto date = TimePoint::now();
	while(TimePoint::now() - date < timeout) {
		for(int x = -size; x <= size; x++)
			for(int y = -size; y <= size; y += size * 2) {
				int posX = gridX + x;
				int posY = gridY + y;
				Vector2m worldPos = this->toWorldPos(posX, posY);
				if(getDangerValueSafeAt(worldPos) < DANGER_INFINITY) {
					return this->toWorldPos(posX, posY);
				}
			}
		size++;
	}


	throw std::runtime_error("Impossible de trouver une position de fuite");
}


float Environment::getDangerValueAt(Vector2m position) const {
	int gridX = toGridUnit(position.x);
	int gridY = toGridUnit(position.y);
	return getDangerValueAt(gridX, gridY);
}

float Environment::getDangerValueAt(int gridX, int gridY) const {
	return _nodes[gridX][gridY].staticValue + _nodes[gridX][gridY].dynamicValue;
}

float Environment::getDangerValueSafeAt(int gridX, int gridY) const {
	if(gridX < 0 || gridY < 0 || gridX >= this->_size.x || gridY >= this->_size.y)
		return DANGER_INFINITY;
	return getDangerValueAt(gridX, gridY);
}

float Environment::getDangerValueSafeAt(Vector2m position) const {
	int gridX = toGridUnit(position.x);
	int gridY = toGridUnit(position.y);
	return getDangerValueSafeAt(gridX, gridY);
}

vector<Vector2m> Environment::linearize(vector<Vector2m> const& trajectory) {
	if(!_postProcessParameters.linearizationEnabled)
		return trajectory;

	vector<Vector2m> newTraj;
	Vector2m last = trajectory.front();
	newTraj.push_back(last);
	for(int i = 1; i < trajectory.size(); i++) {
		// Première condition pour garder le point : il faut que le déplacement en ligne droite
		// du point précédent à ce point soit impossible.
		if(!canMoveLine(last, trajectory[i])) {
			// On ajoute les points s'ils sont indispensables
			newTraj.push_back(trajectory[i - 1]);
			last = trajectory[i];
		}
	}
	newTraj.push_back(trajectory.back());
	return newTraj;
}

vector<Vector2m> Environment::linearizeAngular(vector<Vector2m> const& trajectory) {
	if(!_postProcessParameters.angularLinearization.enabled)
		return trajectory;

	vector<Vector2m> newTraj;

	// Calcul du seuil au carré de la distance.
	Surface distanceThresholdSquared = _postProcessParameters.angularLinearization.discardTreshold *
	                                   _postProcessParameters.angularLinearization.discardTreshold;

	for(int i = 0; i < trajectory.size(); i++) {
		if(i <= 1) {
			newTraj.push_back(trajectory[i]);
		} else {
			// Si il y a déjà des points avant, on peut regarder l'écart angulaire
			// avec le nouveau point pour voir si il est réellement indispensable.
			int lastIndex = newTraj.size() - 1;
			Vector2m m1 = newTraj[lastIndex - 1];
			Vector2m m2 = newTraj[lastIndex];
			Vector2m current = trajectory[i];

			if((m2 - current).squaredNorm() <= distanceThresholdSquared) {
				// newTraj.push_back(trajectory[i]);
				i++;
				continue;
			}

			// Angle
			Angle angle = atan2(m2.y - m1.y, m2.x - m1.x);
			Angle angle2 = atan2(current.y - m1.y, current.x - m1.x);

			// Ecart angulaire important : on doit prendre le point.
			if(abs(angle - angle2) > _postProcessParameters.angularLinearization.threshold) {
				newTraj.push_back(trajectory[i - 1]);
			}
		}
	}
	return newTraj;
}

Vector2m Environment::getCastaljau(vector<Vector2m> const& points, int r, int i, float t) {
	if(r == 0)
		return points[i];

	Vector2m p1 = getCastaljau(points, r - 1, i, t);
	Vector2m p2 = getCastaljau(points, r - 1, i + 1, t);
	return (1 - t) * p1 + t * p2;
}

vector<Vector2m> Environment::bezierize(vector<Vector2m> const& trajectory) {
	if(trajectory.size() <= 2 || !_postProcessParameters.bezier.enabled)
		return trajectory;

	// On décompose le trajet en parties, calculées pour chaque point de la trajectoire.
	// Les parties comportent :
	// - soit 1 seul point pour les extrémités
	// - soit 3 points pour les points intérieurs
	//   ces 3 points correspondent :
	//          - au point actuellement parcouru
	//          - aux 2 points situés au milieu entre le point parcouru et son prédécesseur / successeur.
	std::vector<TrajectoryPart> parts;
	parts.push_back(TrajectoryPart(std::vector<Vector2m>({trajectory[0]}), trajectory[0]));
	for(int i = 1; i < trajectory.size() - 1; i++) {
		Vector2m previous = trajectory[i - 1];
		Vector2m next = trajectory[i + 1];
		Vector2m current = trajectory[i];
		parts.push_back(TrajectoryPart(vector<Vector2m>({(current + previous) / 2, current * 1, (current + next) / 2}), current));
	}
	parts.push_back(TrajectoryPart(vector<Vector2m>({trajectory.back()}), trajectory.back()));

	// Pour chaque partie :
	// Si elle est composée de 3 points : on va effectuer un lissage avec une courbe de bézier.
	// Si cette nouvelle trajectoire est valide => on la prend, sinon, on prend la trajectoire non lissée
	// stockée dans la partie.
	std::vector<Vector2m> newtraj;
	float bezierStep = 1.0f / (float)_postProcessParameters.bezier.pointsCount;
	float bezierDanger = _postProcessParameters.bezier.dangerThreshold;
	for(int i = 0; i < parts.size(); i++) {
		if(parts[i].originalPoints.size() == 1)
			newtraj.push_back(parts[i].middlePoint);
		else {
			// On crée la courbe de bézier à partir des 3 points de notre partie.
			for(float j = 0; j < 1; j += bezierStep) {
				parts[i].bezierizedPoints.push_back(getCastaljau(parts[i].originalPoints, parts[i].originalPoints.size() - 1, 0, j));
			}

			// On vérifie la validité de notre courbe.
			bool keepBezier = true;
			for(int j = 1; j <= parts[i].bezierizedPoints.size() - 1; j++) {
				if(!canMoveLineDanger(parts[i].bezierizedPoints[j - 1], parts[i].bezierizedPoints[j], bezierDanger)) {
					keepBezier = false;
					break;
				}
			}

			// Filtrage angulaire.
			bool canFilter = newtraj.size() > 1;
			if(_postProcessParameters.bezier.angleFilterEnabled && canFilter) {
				int lastIndex = newtraj.size() - 1;
				Vector2m m1 = newtraj[lastIndex - 1];
				Vector2m m2 = newtraj[lastIndex];
				Vector2m current = trajectory[i];

				// Calcul de l'écart angulaire.
				Angle angle = atan2(m2.y - m1.y, m2.x - m1.x);
				Angle angle2 = atan2(current.y - m1.y, current.x - m1.x);

				// Ecart angulaire important : on béziérise.
				keepBezier &= abs(angle - angle2) > _postProcessParameters.bezier.minAngle;
			}

			// On ajoute les points (lissés ou non) à la trajectoire.
			if(keepBezier) {
				for(int j = 0; j <= parts[i].bezierizedPoints.size() - 1; j++) {
					newtraj.push_back(parts[i].bezierizedPoints[j]);
				}
			} else {
				for(int j = 0; j <= parts[i].originalPoints.size() - 1; j++) {
					newtraj.push_back(parts[i].originalPoints[j]);
				}
			}
		}
	}

	return newtraj;
}

bool Environment::canMoveLineDanger(Vector2m start, Vector2m end, float maxDanger) {
	if(start == end)
		return true;

	Vector2m current = start;
	Vector2m direction = end - start;
	Distance totalDistance = direction.norm();

	direction = (direction / totalDistance.toM());

	Distance actualDistance = 0_m;
	Distance step = toWorldUnit(1) * 0.50f;
	// float startDanger = getDangerValueAt(start);

	// On parcours la ligne en plusieurs étapes et on détecte les obstacles éventuels.
	while(actualDistance < totalDistance) {
		actualDistance += step;
		current = start + actualDistance.toM() * direction;
		float danger = getDangerValueAt(current);
		if(danger >= maxDanger) {
			return false;
		}
	}
	return true;
}

bool Environment::canMoveLine(Vector2m start, Vector2m end) {
	if(start == end)
		return true;

	Vector2m current = start;
	Vector2m direction = end - start;
	Distance totalDistance = direction.norm();

	direction = (direction / totalDistance.toM());

	Distance actualDistance = 0_m;
	Distance step = toWorldUnit(1) * 0.50f;
	float startDanger = getDangerValueAt(start);
	// On parcours la ligne en plusieurs étapes et on détecte les obstacles éventuels.
	while(actualDistance < totalDistance) {
		actualDistance += step;
		current = start + actualDistance.toM() * direction;
		float danger = getDangerValueAt(current);
		if(danger > startDanger || danger >= DANGER_INFINITY) {
			return false;
		}
	}
	return true;
}

void Environment::drawStaticShapes(vector<vector<Node>>& target) {
	for(vector<Node>& nodes : target)
		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			it->staticValue = 0;
			it->_debugValue = 0;
		}

	resetNodes(target);

	for(auto& shape : _shapes) {
		drawShape(*shape, target, false);
		// drawDebugShape(*shape, target);
	}
}

void Environment::resetNodes(vector<vector<Node>>& target) {
	for(vector<Node>& nodes : target)
		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			it->dynamicValue = 0;
			it->heuristicCost = 0;
			it->effectiveCost = 0;
		}
}

void Environment::drawDynamicShapes(vector<vector<Node>>& target) {
	for(auto& p : _dynamicShapes)
		drawShape(*(p.second), target, true);
}

void Environment::drawDebugShape(Shape& shape, vector<vector<Node>>& target) {
	if(shape.getType() == ShapeType::Shape_Circle) {
		auto& circle = static_cast<Circle&>(shape);
		Distance radius = circle.radius;
		int sx = max(0, toGridUnit(circle.position.x.toM() - radius.toM()));
		int sy = max(0, toGridUnit(circle.position.y.toM() - radius.toM()));
		int ex = min((int)(target.size() - 1), toGridUnit(circle.position.x.toM() + radius.toM()));
		int ey = min((int)(target[0].size() - 1), toGridUnit(circle.position.y.toM() + radius.toM()));
		auto radiusSqr = radius * radius;
		for(int x = sx; x < ex; x++) {
			for(int y = sy; y < ey; y++) {
				Distance dx = toWorldUnit(x) - circle.position.x;
				Distance dy = toWorldUnit(y) - circle.position.y;
				Surface dstSqr = dx * dx + dy * dy;

				if(dstSqr <= radiusSqr)
					target[x][y]._debugValue += circle.value;
			}
		}
	} else if(shape.getType() == ShapeType::Shape_Rectangle) {
		Rect& rect = static_cast<Rect&>(shape);
		int sx = max(0, toGridUnit(rect.position.x));
		int sy = max(0, toGridUnit(rect.position.y));
		int ex = min((int)target.size() - 1, toGridUnit(rect.position.x + rect.size.x));
		int ey = min((int)target[0].size() - 1, toGridUnit(rect.position.y + rect.size.y));
		for(int x = sx; x < ex; x++) {
			for(int y = sy; y < ey; y++) {
				target[x][y]._debugValue += rect.value;
			}
		}
	}
}

void Environment::drawShape(Shape& shape, vector<vector<Node>>& target, bool dynamic) {
	if(shape.getType() == ShapeType::Shape_Circle) {
		Circle& circle = static_cast<Circle&>(shape);

		// Rayon de l'obstacle :
		// on prend en compte le rayon du robot
		Distance radius = circle.radius + _robotWidth;
		Distance outterRadius = circle.radius + _radius;

		int sx = max(0, toGridUnit(circle.position.x.toM() - outterRadius.toM()));
		int sy = max(0, toGridUnit(circle.position.y.toM() - outterRadius.toM()));
		int ex = min((int)(target.size() - 1), toGridUnit(circle.position.x.toM() + outterRadius.toM()));
		int ey = min((int)(target[0].size() - 1), toGridUnit(circle.position.y.toM() + outterRadius.toM()));

		auto outterRadiusSqr = outterRadius * outterRadius;
		auto radiusSqr = radius * radius;
		for(int x = sx; x <= ex; x++) {
			for(int y = sy; y <= ey; y++) {
				Distance dx = toWorldUnit(x) - circle.position.x;
				Distance dy = toWorldUnit(y) - circle.position.y;
				Surface dstSqr = dx * dx + dy * dy;
				Distance dst = sqrt(dstSqr);
				if(dynamic) {
					if(dstSqr <= radiusSqr)
						target[x][y].dynamicValue += circle.value;
					else if(dstSqr <= outterRadiusSqr) {
						target[x][y].dynamicValue += lerp(circle.value, 0, (dst - radius) / (outterRadius - radius));
					}
				} else {
					if(dstSqr <= radiusSqr)
						target[x][y].staticValue += circle.value;
					else if(dstSqr <= outterRadiusSqr) {
						target[x][y].staticValue += lerp(circle.value, 0, (dst - radius) / (outterRadius - radius));
					}
				}
			}
		}
	} else if(shape.getType() == ShapeType::Shape_Rectangle) {
		Rect& rect = static_cast<Rect&>(shape);
		// On prend en compte le rayon du robot dans l'obstacle !
		this->drawRect(toGridUnit(rect.position.x - this->_robotWidth),
		               toGridUnit(rect.position.y - this->_robotWidth),
		               toGridUnit(rect.size.x + this->_robotWidth * 2),
		               toGridUnit(rect.size.y + this->_robotWidth * 2),
		               toGridUnit(rect.angle),
		               rect.value,
		               dynamic);

		// On ne dessine pas les rectangles et quarts de cercle avec un gradient TODO : à implémenter
		if(rect.angle == 0_deg) {
			int diff = toGridUnit(this->_radius - this->_robotWidth);

			// On dessine la zone de danger modérée (largeur : diagonale du robot)
			float gradMaxDanger = rect.value;
			float gradMinDanger = 0;
			// Haut
			this->drawGradientRect(toGridUnit(rect.position.x - this->_radius) + diff,
			                       toGridUnit(rect.position.y - this->_radius),
			                       toGridUnit(rect.size.x + this->_radius * 2) - diff * 2,
			                       diff,
			                       GRADIENT_RECT_TOP,
			                       gradMaxDanger,
			                       gradMinDanger,
			                       dynamic);
			// Gauche
			this->drawGradientRect(toGridUnit(rect.position.x - this->_radius),
			                       toGridUnit(rect.position.y - this->_radius) + diff,
			                       diff,
			                       toGridUnit(rect.size.y + this->_radius * 2) - diff * 2,
			                       GRADIENT_RECT_LEFT,
			                       gradMaxDanger,
			                       gradMinDanger,
			                       dynamic);

			// Bas
			this->drawGradientRect(toGridUnit(rect.position.x - this->_radius) + diff,
			                       toGridUnit(rect.position.y + rect.size.y + this->_robotWidth),
			                       toGridUnit(rect.size.x + this->_radius * 2) - diff * 2,
			                       diff,
			                       GRADIENT_RECT_BOT,
			                       gradMaxDanger,
			                       gradMinDanger,
			                       dynamic);


			// Droite
			this->drawGradientRect(toGridUnit(rect.position.x + rect.size.x + this->_robotWidth),
			                       toGridUnit(rect.position.y - this->_radius) + diff,
			                       diff,
			                       toGridUnit(rect.size.y + this->_radius * 2) - diff * 2,
			                       GRADIENT_RECT_RIGHT,
			                       gradMaxDanger,
			                       gradMinDanger,
			                       dynamic);


			// Dessine les arrondis
			this->drawQuadrant(toGridUnit(rect.position.x - this->_robotWidth),
			                   toGridUnit(rect.position.y - this->_robotWidth),
			                   diff,
			                   QUADRANT_UPPER_LEFT,
			                   gradMinDanger,
			                   gradMaxDanger,
			                   dynamic);
			this->drawQuadrant(toGridUnit(rect.position.x + rect.size.x + this->_robotWidth),
			                   toGridUnit(rect.position.y - this->_robotWidth),
			                   diff,
			                   QUADRANT_UPPER_RIGHT,
			                   gradMinDanger,
			                   gradMaxDanger,
			                   dynamic);

			this->drawQuadrant(toGridUnit(rect.position.x + rect.size.x + this->_robotWidth),
			                   toGridUnit(rect.position.y + rect.size.y + this->_robotWidth),
			                   diff,
			                   QUADRANT_LOWER_RIGHT,
			                   gradMinDanger,
			                   gradMaxDanger,
			                   dynamic);

			this->drawQuadrant(toGridUnit(rect.position.x - this->_robotWidth),
			                   toGridUnit(rect.position.y + rect.size.y + this->_robotWidth),
			                   diff,
			                   QUADRANT_LOWER_LEFT,
			                   gradMinDanger,
			                   gradMaxDanger,
			                   dynamic);
		}
	}
}
/** Dessine un rectangle sur la grille
 * @param angle rotation autour du sommet en haut à gauche dans le sens trigonométrique
 * @param dangerValue valeur de danger à mettre dans le rectangle
 * @param dynamic si vrai : on incrit la valeur en temps qu'obstacle dynamique.*/
void Environment::drawRect(int rx, int ry, int w, int h, float angle, float dangerValue, bool dynamic) {
	// on fait en sorte que 'angle' soit compris dans l'intervalle [0 ; 2*M_PI]
	while(angle < 0)
		angle += 2 * M_PI;

	while(angle > 2 * M_PI)
		angle -= 2 * M_PI;

	bool rectDroit = angle == 0;
	if(abs(angle - M_PI_2) < 0.0001) {
		ry -= w;
		int temp = w;
		w = h;
		h = temp;
		rectDroit = true;
	} else if(abs(angle - M_PI) < 0.0001) {
		rx -= w;
		ry -= h;
		rectDroit = true;
	} else if(abs(angle - 3 * M_PI_2) < 0.0001) {
		rx -= h;
		int temp = w;
		w = h;
		h = temp;
		rectDroit = true;
	}

	if(rectDroit) {
		// Calcul des bornes du rectangle
		int sx = min(this->_size.x - 1, max(rx, 0));
		int sy = min(this->_size.y - 1, max(ry, 0));
		int ex = min(this->_size.x - 1, max(rx + w, 0));
		int ey = min(this->_size.y - 1, max(ry + h, 0));

		// Dessin
		for(int x = sx; x <= ex; x++) {
			for(int y = sy; y <= ey; y++) {
				if(dynamic)
					this->_nodes[x][y].dynamicValue = max(dangerValue, this->_nodes[x][y].dynamicValue);
				else
					this->_nodes[x][y].staticValue = max(dangerValue, this->_nodes[x][y].staticValue);
			}
		}
	} else {
		// calcul des coordonnées des sommets du rectangle incliné
		float Ax = rx;
		float Ay = ry;
		float Bx = Ax + sin(angle) * h;
		float By = Ay + cos(angle) * h;
		float Dx = Ax + cos(angle) * w;
		float Dy = Ay - sin(angle) * w;
		float Cx = Bx + Dx - Ax;
		float Cy = By + Dy - Ay;

		// équation des droites respectivement du 'haut', de 'gauche', du 'bas' et de 'droite'
		float mD1 = (Ay - Dy) / (Ax - Dx);
		float pD1 = Ay - mD1 * Ax;
		float mD2 = (Ay - By) / (Ax - Bx);
		float pD2 = Ay - mD2 * Ax;
		float mD3 = (By - Cy) / (Bx - Cx);
		float pD3 = By - mD3 * Bx;
		float mD4 = (Cy - Dy) / (Cx - Dx);
		float pD4 = Cy - mD4 * Cx;

		// coordonnées du plus petit rectangle selon le repère (ex, ey) qui comprend le rectangle à dessiner penché
		float minX = max(min({Ax, Bx, Cx, Dx}), (float)0);
		float maxX = min(max({Ax, Bx, Cx, Dx}), (float)this->_size.x - 1);
		float minY = max(min({Ay, By, Cy, Dy}), (float)0);
		float maxY = min(max({Ay, By, Cy, Dy}), (float)this->_size.y - 1);

		// on dessine tous les points s'ils sont à l'intérieur des 4 droites définies par les points du sommet
		for(float x = minX; x <= maxX; x += 0.4) {
			for(float y = minY; y <= maxY; y += 0.4) {
				if((((angle < M_PI_2 or angle > 3 * M_PI_2) and y >= mD1 * x + pD1) or
				    (angle > M_PI_2 and angle < 3 * M_PI_2 and y <= mD1 * x + pD1)) and
				   ((angle > M_PI and y >= mD2 * x + pD2) or (angle < M_PI and y <= mD2 * x + pD2)) and
				   (((angle < M_PI_2 or angle > 3 * M_PI_2) and y <= mD3 * x + pD3) or
				    (angle > M_PI_2 and angle < 3 * M_PI_2 and y >= mD3 * x + pD3)) and
				   ((angle > M_PI and y <= mD4 * x + pD4) or (angle < M_PI and y >= mD4 * x + pD4))) {

					if(dynamic)
						this->_nodes[x][y].dynamicValue = max(dangerValue, this->_nodes[x][y].dynamicValue);
					else
						this->_nodes[x][y].staticValue = max(dangerValue, this->_nodes[x][y].staticValue);
				}
			}
		}
	}
}

/**
 * Dessine un rectangle en dégradé sur la grille.
 * @param gradientDir direction du gradient : 1 => haut, 2 => gauche, 3 => bas, 4 => droite
 */
void Environment::drawGradientRect(int rx, int ry, int w, int h, int gradientDir, float maxDanger, float minDanger, bool dynamic) {
	// Calcul des bornes du rectangle
	int sx = min(this->_size.x - 1, max(rx, 0));
	int sy = min(this->_size.y - 1, max(ry, 0));
	int ex = min(this->_size.x - 1, max(rx + w, 0));
	int ey = min(this->_size.y - 1, max(ry + h, 0));


	// Dessin
	for(int x = sx; x <= ex; x++) {
		for(int y = sy; y <= ey; y++) {
			float dangerValue;
			switch(gradientDir) {
				case GRADIENT_RECT_TOP:
					dangerValue = lerp(minDanger, maxDanger, (float)(y - sy) / (float)max(1, ey - sy));
					break;
				case GRADIENT_RECT_LEFT:
					dangerValue = lerp(minDanger, maxDanger, (float)(x - sx) / (float)max(1, ex - sx));
					break;
				case GRADIENT_RECT_BOT:
					dangerValue = lerp(maxDanger, minDanger, (float)(y - sy) / (float)max(1, ey - sy));
					break;
				case GRADIENT_RECT_RIGHT:
					dangerValue = lerp(maxDanger, minDanger, (float)(x - sx) / (float)max(1, ex - sx));
					break;
				default:
					throw runtime_error("erreur");
			}

			if(dynamic)
				this->_nodes[x][y].dynamicValue = max(dangerValue, this->_nodes[x][y].dynamicValue);
			else
				this->_nodes[x][y].staticValue = max(dangerValue, this->_nodes[x][y].staticValue);
		}
	}
}
/**
 * Dessine un quart de cercle sur la grille :
 * @param radius rayon du quart de cercle
 * @param quadrant numéro du quadrant. 1 : supérieur droit, 2 : supérieur gauche, 3 : inférieur gauche, 4 : inférieur
 * droit
 * @param valeur à mettre au centre du quadrant
 * @param valeur à mettre à l'extrémité du quadrant
 * */
void Environment::drawQuadrant(int cx, int cy, int radius, int quadrant, float maxValue, float minValue, bool dynamic) {
	const int offsets[][4] = {
	    {cx, cy - radius, cx + radius, cy}, // supérieur droit
	    {cx - radius, cy - radius, cx, cy}, // supérieur gauche
	    {cx - radius, cy, cx, cy + radius}, // inférieur gauche
	    {cx, cy, cx + radius, cy + radius}  // inférieur droit
	};

	// On calcule les bornes du rectangle dans lequel est inscrit le cercle.
	int sx = max(0, offsets[quadrant][0]);
	int sy = max(0, offsets[quadrant][1]);
	int ex = min(this->_size.x - 1, offsets[quadrant][2]);
	int ey = min(this->_size.y - 1, offsets[quadrant][3]);

	auto radiusSqr = radius * radius;
	for(int x = sx; x <= ex; x++) {
		for(int y = sy; y <= ey; y++) {
			// On calcule le carré de la distance entre le centre du cercle et le point actuel.
			float dx = x - cx;
			float dy = y - cy;
			float dstSqr = (dx * dx + dy * dy);
			float dst = sqrt(dstSqr);
			// On skip le point s'il n'est pas dans le cercle.
			if(dstSqr > radiusSqr)
				continue;

			// On fait le dégradé en faisant une interpolation linéaire en fonction de
			// la distance du point calculé au centre du cercle.
			float value = lerp(minValue, maxValue, dst / (float)radius);

			if(dynamic)
				this->_nodes[x][y].dynamicValue = max(value, this->_nodes[x][y].dynamicValue);
			else
				this->_nodes[x][y].staticValue = max(value, this->_nodes[x][y].staticValue);
		}
	}
}
void Environment::addStaticShape(unique_ptr<Shape> shape) {
	_shapes.push_back(move(shape));
	_staticDirty = true;
}

void Environment::clearStaticShapes() {
	_shapes.clear();
}


void Environment::addDynamicShape(int index, unique_ptr<Shape> shape) {
	removeDynamicShape(index); // FIXME : remove dat
	_dynamicShapes[index] = move(shape);
	_dynamicDirty = true;
}

void Environment::removeDynamicShape(int index) {
	if(_dynamicShapes.count(index) > 0)
		_dynamicShapes.erase(index);

	_dynamicDirty = true;
}

void Environment::setGridSize(Vector2u16 size) {
	_size = size;
	_staticDirty = true;
	_dynamicDirty = true;
	initNodes();
}

int Environment::toGridUnit(float value) const {
	return (int)(value / _scale.toM());
}

Distance Environment::toWorldUnit(float value) {
	return value * _scale;
}

Vector2m Environment::toWorldPos(int x, int y) {
	return Vector2m(toWorldUnit(x), toWorldUnit(y));
}


float Environment::lerp(float a1, float a2, float v) {
	return (a2 * v) + (a1 * (1 - v));
}


void Environment::saveToTGA(const char* path, vector<Vector2m> const& traj) const {
	// void exportTGA(Environment const& astar, vector<Vector2m> const& pts, int w, int h)
	// 0xaarrggbb
	const int ma = 0xFF000000;
	const int mr = 0x00FF0000;
	const int mg = 0x0000FF00;
	const int mb = 0x000000FF;
	char r, g, b, a = 0;

	TGAWriter writer(this->_size.x, this->_size.y);
	const vector<vector<Node>>& obst = this->getNodes();

	// On inverse tous les points selon l'axe Y pour que l'affichage soit bien orienté
	std::function<Vector2m(Vector2m)> inverserY = [&writer, this](Vector2m vec) {
		return Vector2m(vec.x, writer.getHeight() * _scale - vec.y);
	};
	// fill
	for(int x = 0; x < writer.getWidth(); x++) {
		for(int y = 0; y < writer.getHeight(); y++) {
			r = g = b = a = 0;
			float value = obst[x][y].staticValue + obst[x][y].dynamicValue;
			if(value > 0) {
				a = b = r = (char)255;

				// Affichage des dangers infinis en noir
				if(value == DANGER_INFINITY) {
					b = g = 0;
					r = (char)200;
				} else {
					// Formule un peu bizarre qui permet d'avoir un joli dégradé --> correction gamma
					g = r = (char)(255 * pow((255 - max(0, min(255, (int)(value * (255.0f / DANGER_INFINITY))))) / 255.0, 1 / 2.2));
				}

			} else {
				r = g = b = a = (char)255;
			}

			// Affichage de la debug value
			/*if(false) {
			    if(obst[x][y]._debugValue > 0) {
			        a = 255;
			        g = (char)(min((int)obst[x][y]._debugValue, (int)255));
			    }
			}*/

			writer.drawPoint((((int)a << 24) & ma) | (((int)r << 16) & mr) | (((int)g << 8) & mg) | (b & mb), x, writer.getHeight() - y);
		};
	}

	// trajectoire.
	float invGS = 1.0f / this->_scale.toM();
	for(int j = 1; j < traj.size(); j++) {
		auto trajPoint = traj[j];
		// auto ptUnit = traj[j] * invGS;
		writer.drawLine(0xFF00FF00, ((inverserY(traj[j] + _offset)) * invGS), ((inverserY(traj[j - 1] + _offset)) * invGS));
	}

	for(int j = 0; j < traj.size(); j++) {

		auto ptUnit = (traj[j] + _offset) * invGS;
		if(j == 0) {
			// ARRIVEE
			for(int x = 0; x < 3; x++) {
				for(int y = 0; y < 3; y++) {
					writer.drawPoint(0xFFFFA000, (int)ptUnit.x.toM() + x, writer.getHeight() - (int)ptUnit.y.toM() - y);
				}
			}
		} else if(j == traj.size() - 1) {
			// DEPART
			for(int x = 0; x < 4; x++) {
				for(int y = 0; y < 4; y++) {
					writer.drawPoint(0xFF00A0FF, (int)ptUnit.x.toM() + x, writer.getHeight() - (int)ptUnit.y.toM() + y);
				}
			}
		} else
			writer.drawPoint(0xFF0000FF, (int)ptUnit.x.toM(), writer.getHeight() - (int)ptUnit.y.toM());
	}


	writer.save(path);
}

Distance Environment::getScale() {
	return this->_scale;
}

TrajectoryPostProcessParameterSet& Environment::getPostProcessParameters() {
	return _postProcessParameters;
}

vector<Vector2m> Environment::removeOffset(vector<Vector2m> const& trajectory) {
	vector<Vector2m> vec;
	for(auto& v : trajectory) {
		vec.push_back(v - _offset);
	}
	return vec;
}

vector<Vector2m> Environment::postProcess(vector<Vector2m> const& trajectory, bool cubic_interpolation) {
	// Si on calcule les cubiques, on ne veut pas avoir plein de points non essentiels
	if(cubic_interpolation)
		return removeOffset(linearizeAngular(bezierize(linearize(trajectory))));
	// Sinon on rajoute toutes les étapes pour linéariser un maximum la trajectoire
	else
		return removeOffset(performSegmentation(linearizeAngular(bezierize(linearize(trajectory)))));
}

vector<Vector2m> Environment::performSegmentation(vector<Vector2m> const& trajectory) {
	if(!_postProcessParameters.trajectorySegmentation.enabled)
		return trajectory;
	vector<Vector2m> newtraj;
	Surface threshold =
	    _postProcessParameters.trajectorySegmentation.maxSpacing * _postProcessParameters.trajectorySegmentation.maxSpacing;

	newtraj.push_back(trajectory.front());
	for(int i = 1; i < trajectory.size(); i++) {
		Vector2m previous = trajectory[i - 1];
		Vector2m current = trajectory[i];
		Surface squaredNorm = (previous - current).squaredNorm();

		// Si on dépasse le seuil de distance max
		if(squaredNorm > threshold) {
			// On décompose le segment en parts égales.
			Distance norm = sqrt(squaredNorm);
			int steps = (int)(norm / _postProcessParameters.trajectorySegmentation.maxSpacing) + 2;
			Vector2m stepDistance = (current - previous) / steps;
			for(int j = 1; j <= steps; j++) {
				newtraj.push_back(previous + stepDistance * j);
			}
		} else {
			newtraj.push_back(current);
		}
	}

	return newtraj;
}

Distance Environment::getRobotRadius() {
	return this->_radius;
}

void Environment::clearDebugValues() {
	for(vector<Node>& nodes : _nodes)
		for(auto it = nodes.begin(); it != nodes.end(); it++) {
			it->_debugValue = 0;
		}
}

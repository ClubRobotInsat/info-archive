#pragma once
#include "../../../commun/Commun.h"
#include "node.h"
#include "shapes.h"
#include <Json.h>
#include <algorithm>
#include <cmath>
#include <exception>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <queue>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Neighbor {
	Neighbor(Distance distance, Node& n);

	/** Représente la distance au point pour lequel ce voisin a été sélectionné. */
	Distance dist;
	/** Pointeur vers le noeud représenté par ce voisin. */
	Node& n;
};

struct TrajectoryPart {
	std::vector<Vector2m> originalPoints;
	Vector2m middlePoint;
	std::vector<Vector2m> bezierizedPoints;
	TrajectoryPart(std::vector<Vector2m> const& originalpoints_, Vector2m middlepoint_)
	        : originalPoints(originalpoints_), middlePoint(middlepoint_) {}
};


/**
 * Décrit les paramètres de l'étape de "bésiériation" de la trajectoire.
 */
struct BezierizationParameterSet {
	/**
	 * Indique si la béziérisation de trajectoire est activée
	 */
	bool enabled;
	/**
	 * Nombre de points pour chaque courbe de bézier
	 * Plus ce nombre est élevé, plus la trajectoire sera courbée.
	 */
	int pointsCount;
	/**
	 * Seuil de danger que la portion de trajectoire "bériérisée" ne doit pas dépasser.
	 * Si cette portion dépasse ce seuil de danger, elle est remplacée par la portion originale.
	 */
	float dangerThreshold;


	/**
	 * Indique si le filtrage angulaire est activé.
	 * S'il est activé, un angle minimum (minAngle) sera exigé pour qu'une
	 * portion de trajectoire soit bézirisée.
	 */
	bool angleFilterEnabled;
	/**
	 * Angle minimum de la portion de trajectoire à bésiériser.
	 * Si l'angle de cette portion de trajectoire est inférieur à minAngle, on
	 * ne béziérise pas.
	 * Angle de portion élevé     |  Angle de portion faible
	 *       ---x---------------x |
	 *    ---                     |        x-----------------x
	 * x---                       |  x------
	 * _______________________________________________________
	 * => Besoin de bésiériser    | Pas besoin
	 */
	Angle minAngle;
};

/**
 * Décrit les paramètres de la linéarisation angulaire.
 * Cette linéarisation permet de réduire le nombre de points de la trajectoire,
 * mais ses paramètres doivent être bien choisis, car elle ne garantit pas
 * (si les paramètres sont mal choisis) que la trajectoire résultante
 * ne traverse pas un mur !
 */
struct AngularLinearizationParameterSet {
	/**
	 * Indique si la linéarisation angulaire de la trajectoire est activée.
	 */
	bool enabled;
	/**
	 * Ecart angulaire à partir duquel un point est considéré indispensable.
	 * Une valeur basse (1 ou 2 degrés) permet une meilleure précision aux prix
	 * d'un nombre de points + important.
	 * Une valeur trop haute peut donner lieu à une trajectoire non valide.
	 */
	Angle threshold;
	/**
	 * Distance en-deça de laquelle 2 points proches sont mergés.
	 */
	Distance discardTreshold;
};

/**
 * Décrit les paramètres de la segmentation de trajectoire.
 * La segmentation de trajectoire ne change pas la trajectoire prise au final,
 * mais modifie l'espacement des points les uns par rapport aux autres.
 *
 * Ceci permet de prendre en compte des contraintes de correction angulaire du robot.
 */
struct TrajectorySegmentationParameterSet {
	/**
	 * Indique si la segmentation de trajectoire est activée.
	 */
	bool enabled;
	/**
	 * Représente la distance maximale entre 2 points.
	 */
	Distance maxSpacing;
};

/**
 * Set de paramètres pour le post-processing de la trajectoire.
 */
struct TrajectoryPostProcessParameterSet {
	bool linearizationEnabled;
	BezierizationParameterSet bezier;
	AngularLinearizationParameterSet angularLinearization;
	TrajectorySegmentationParameterSet trajectorySegmentation;
};

class Environment {
public:
	static const float DANGER_INFINITY;

	/**
	 * Crée une nouvelle instance de l'A*.
	 * @param size taille de la grille (cases).
	 * @param scale taille en mètres d'une case de la grille.
	 * @param offset différence entre le (0, 0) du robot et le (0, 0) de l'environnement.
	 * 		  Utile si le (0, 0) est au mileu en y de la carte : passer (0, 1_m) en offset.
	 */
	Environment(Vector2u16 size, Distance scale, Distance robotWidth, Distance robotRadius, Vector2m offset = {0_m, 0_m});
	/**
	 * Obtient la grille des noeuds de l'A*.
	 */
	Nodemap const& getNodes() const;
	/**
	 * Obtient la taille de l'A* en cases.
	 */
	Vector2u16 getSize() const {
		return this->_size;
	}
	/**
	 * Obtient l'offset de coordonnées de l'A* par rapport au monde réel.
	 */
	Vector2m getOffset() const {
		return this->_offset;
	}
	/**
	 * Obtient la valeur du rayon du robot tel que passé en paramètre dans l'A*.
	 */
	Distance getRobotRadius();
	/**
	 * Obtient une référence sur les paramètres du post-processing de la trajectoire.
	 */
	TrajectoryPostProcessParameterSet& getPostProcessParameters();

	/** Initialise le graphe de noeuds.	*/
	void initNodes();

	/**
	 * Ajoute une forme statique dans l'environnement de l'A*.
	 * (Note : les dimensions de la forme sont en mètres).
	 */
	void addStaticShape(std::unique_ptr<Shape> shape);

	/**
	 * Supprime toutes les formes statiques de l'environnement de l'A*.
	 */
	void clearStaticShapes();
	/**
	 * Ajoute une forme dynamique dans l'environnement de l'A*.
	 * Si la forme d'index donné existe déjà, elle est remplacée.
	 * (Note : les dimensions de la forme sont en mètres).
	 */
	void addDynamicShape(int index, std::unique_ptr<Shape> shape);
	/**
	 * Supprime la forme dynamque d'index donné si elle existe.
	 */
	void removeDynamicShape(int index);

	/**
	 * Obtient la trajectoire du point de début jusqu'au point de fin.
	 * @param start position du départ de la trajectoire.
	 * @param end position de fin de la trajectoire.
	 * @param cubic_interpolation permet de calculer l'interpolation par splines cubiques pour lisser la trajectoire
	 * @param maxSteps détermine le nombre maximal de noeuds explorés par l'A*. -1 => infini.
	 */
	std::vector<Vector2m>
	    getTrajectory(Vector2m start, Vector2m end, Duration timeout, bool cubic_interpolation = false, int maxSteps = -1);

	/**
	 * Effectue une sauvegarde de l'environnement (formes) dans un fichier tga.
	 */
	void saveToTGA(const char* path, std::vector<Vector2m> const& traj) const;
	Distance getScale();
	void clearDebugValues();

	bool isForbidden(Vector2m const& position) const;

	bool loadFromFile(std::string filename);
	void loadFromJSON(JSON json);

private:
	/**
	 * Obtient le coût heuristique d'un noeud vers l'autre.
	 * @param from noeud source.
	 * @param to noeud de destination.
	 */
	float getHeuristicCost(Node& from, Node& to);

	/**
	 * Ajoute le voisin aux coordonnées (x, y) et à la distance donnée à la liste des voisins,
	 * SI ce voisin existe. (fonction utilisée seulement dans getNeighbors).
	 */
	void addNeighbor(std::vector<Neighbor>& neighbors, int x, int y, Distance distance);
	/**
	 * Obtient les voisins du noeud donné.
	 * @param noeud duquel donner les voisins.
	 */
	std::vector<Neighbor> getNeighbors(Node& n);
	/**
	 * Obtient les étapes permettant de relier le noeud source et destination.
	 * @param from noeud source
	 * @param to de destination
	 * @param previousNodes map contenant pour chaque noeud le noeud qui a été utilisé pour l'atteindre.
	 */
	std::vector<Vector2m> getPathTo(Node& from, Node& to, std::unordered_map<Node*, Node*>& previousNodes);

	/** Obtient une position permettant de sortir d'une zone de danger infini. */
	Vector2m getEscapePosition(Vector2m start, Duration timeout);

	/**
	 * Dessine toutes les formes statiques sur la grille.
	 */
	void drawStaticShapes(std::vector<std::vector<Node>>& target);


	/**
	 * Remet les noeuds donnés à leur état initial.
	 */
	void resetNodes(std::vector<std::vector<Node>>& target);

	/**
	 * Dessine les formes dynamiques.
	 * /!\ Un appel à resetNodes préalable est nécessaire.
	 */
	void drawDynamicShapes(std::vector<std::vector<Node>>& target);

	/**
	 * Dessine la forme donnée sur la grille de noeuds donnés.
	 * @param shape forme à dessiner.
	 * @param target grille sur laquelle dessiner les formes.
	 * @param dynamic booléen indiquant si la forme à dessiner est dynamique.
	 */
	void drawShape(Shape& shape, std::vector<std::vector<Node>>& target, bool dynamic);

	/**
	 * Dessine la forme + une forme de debug (qui apporte des infos supplémentaires).
	 */
	void drawDebugShape(Shape& shape, std::vector<std::vector<Node>>& target);

	/**
	 * Modifie la taille de la grille.
	 */
	void setGridSize(Vector2u16 size);
	/**
	 * Obtient une valeur indiquant si la ligne donnée est praticable
	 * par le robot.
	 * @param
	 */
	bool canMoveLine(Vector2m start, Vector2m end);
	/**
	 * Même chose que canMoveLine.
	 * Différence : le mouvement est autorisé dans des zones de danger croissantes MAIS
	 * interdit seulement dans les zones de danger infini (comme pour canMoveLine).
	 */
	bool canMoveLineDanger(Vector2m start, Vector2m end, float maxDanger);
	/**
	 * Linéarise une trajectoire.
	 * La linéarisation permet de palier le fait que l'A* utilise une grille et
	 * que les déplacements du robots dans cette grille s'effectuent de case en case
	 * (=> 8 orientations possibles seulement).
	 * Cette linéarisation ne que garde les points :
	 * 		- qui peuvent être rejoints en ligne droite sans traverser d'obstacle
	 * 		- qui forment des lignes dont le coût est garanti inférieur au coût calculé
	 * 		par l'A* (les lignes ne traversent pas des zones de dangers croissantes).
	 * Ex :
	 *
	 *     x x x x x x x        =>                 --x
	 *    x                     =>             ----
	 *   x                      =>         ----
	 *  x                       =>     ----
	 * x                        =>   x
	 * @param trajectory trajectoire à linéariser.
	 */
	std::vector<Vector2m> linearize(std::vector<Vector2m> const& trajectory);
	/**
	 * Effectue une linéarisation angulaire de la trajectoire.
	 * Cette linéarisation s'applique après la première passe de linearize, et permet de
	 * virer des points que la première passe a conservé car ils étaient sur une zone de danger
	 * croissante, mais qui ne sont pas utiles car sont alignés les uns avec les autres.
	 * Ex
	 *
	 *            x - - - - x   =>              - - - - - - x
	 * x - - -  x                   x - - - - -
	 */
	std::vector<Vector2m> linearizeAngular(std::vector<Vector2m> const& trajectory);

	Vector2m getCastaljau(std::vector<Vector2m> const& points, int r, int i, float t);
	/**
	 * Effectue un lissage de la trajectoire en utilisant des courbe de bézier.
	 * Ce lissage a pour but de diminuer le nombre de virages avec des angles
	 * importants.
	 */
	std::vector<Vector2m> bezierize(std::vector<Vector2m> const& trajectory);

	/**
	 * Applique la segmentation de trajectoire sur la trajectoire donnée.
	 *
	 * La segmentation de trajectoire ne change pas la trajectoire prise au final,
	 * mais modifie l'espacement des points les uns par rapport aux autres.
	 *
	 * Ex :
	 *
	 * x-----------------------------------------------x
	 *                      |
	 *                      V
	 * x----x----x----x----x----x----x----x----x----x--x
	 */
	std::vector<Vector2m> performSegmentation(std::vector<Vector2m> const& trajectory);

	/**
	 * Retourne la liste de points donnée en paramètre avec l'offset supprimé..
	 */
	std::vector<Vector2m> removeOffset(std::vector<Vector2m> const& trajectory);

	/**
	 * Effectue les opérations de post-processing sur la trajectoire passée en paramètre.
	 * @param cubic_interpolation permet de calculer l'interpolation par splines cubiques pour lisser la trajectoire
	 */
	std::vector<Vector2m> postProcess(std::vector<Vector2m> const& trajectory, bool cubic_interpolation = false);

	/**
	 * Obtient la valeur de danger à la position donnée.
	 */
	float getDangerValueAt(Vector2m position) const;
	float getDangerValueAt(int gridX, int gridY) const;
	/**
	 * Obtient la valeur de danger à la position de grille donnée.
	 * Cette opération s'effectue avec des checks. (=> failsafe)
	 */
	float getDangerValueSafeAt(int gridX, int gridY) const;
	float getDangerValueSafeAt(Vector2m position) const;


	/**
	 * Dessine un rectangle sur la grille
	 * @param angle rotation autour du sommet en haut à gauche dans le sens trigonométrique
	 * @param dangerValue valeur de danger à mettre dans le rectangle
	 * @param dynamic si vrai : on incrit la valeur en temps qu'obstacle dynamique.*/
	void drawRect(int x, int y, int w, int h, float angle, float dangerValue, bool dynamic);
	/**
	 * Dessine un rectangle en dégradé sur la grille.
	 * @param gradientDir direction du gradient : 1 => haut, 2 => gauche, 3 => bas, 4 => droite
	 */
	void drawGradientRect(int x, int y, int w, int h, int gradientDir, float maxDanger, float minDanger, bool dynamic);
	/**
	 * Dessine un quart de cercle sur la grille :
	 * @param radius rayon du quart de cercle
	 * @param quadrant numéro du quadrant. 1 : supérieur droit, 2 : supérieur gauche, 3 : inférieur gauche, 4 :
	 * inférieur droit
	 * @param valeur à mettre au centre du quadrant
	 * @param valeur à mettre à l'extrémité du quadrant
	 * */
	void drawQuadrant(int cx, int cy, int radius, int quadrant, float maxValue, float minValue, bool dynamic);
	int toGridUnit(float value) const;
	Distance toWorldUnit(float value);
	Vector2m toWorldPos(int x, int y);
	static float lerp(float a1, float a2, float v);
	// Des surchages...
	Distance lerp(Distance a1, Distance a2, float v) {
		return Distance::makeFromM(lerp(a1.toM(), a2.toM(), v));
	}
	int toGridUnit(Distance value) const {
		return toGridUnit(value.toM());
	}
	float toGridUnit(Angle value) const {
		return (float)value.toRad();
	}

private:
	static const int GRADIENT_RECT_TOP = 1;
	static const int GRADIENT_RECT_LEFT = 2;
	static const int GRADIENT_RECT_BOT = 3;
	static const int GRADIENT_RECT_RIGHT = 4;
	static const int QUADRANT_UPPER_RIGHT = 0;
	static const int QUADRANT_UPPER_LEFT = 1;
	static const int QUADRANT_LOWER_LEFT = 2;
	static const int QUADRANT_LOWER_RIGHT = 3;
	/** Paramètres du post-processing de la trajectoire */
	TrajectoryPostProcessParameterSet _postProcessParameters;
	/** Taille de la grille, en cases. */
	Vector2u16 _size;
	/** Grille utilisée par l'A* */
	Nodemap _nodes;
	/** Formes statiques en place sur la grille. */
	std::vector<std::unique_ptr<Shape>> _shapes;
	/** Formes dynamiques en place sur la grille.*/
	std::map<int, std::unique_ptr<Shape>> _dynamicShapes;
	/**
	 * Indique si la liste de formes statiques a changé depuis le dernier calcul
	 * de l'A*.
	 */
	bool _staticDirty;
	/**
	 * Indique si la liste de formes dynamiques a changé depuis le dernier calcul
	 * de l'A*.
	 */
	bool _dynamicDirty;

	/**
	 * Rayon du robot en unités métriques.
	 */
	Distance _radius;
	/**
	 * Demi-largeur du robot en unités métriques.
	 */
	Distance _robotWidth;
	/**
	 * Taille en mètre d'une unité de longueur de la grille de l'A*.
	 */
	Distance _scale;

	/**
	 * Différence entre le (0, 0) du robot et le (0, 0) de l'environnement.
	 * Utile si le (0, 0) est au milieu en y de la carte : passer (0, 1_m) en offset.
	 */
	Vector2m _offset;
};

//
//  Deplacement.cpp
//  Club Robot
//
//  Created by Rémi on 18/12/2014.
//

#include "../../commun/MathToolbox/MathToolbox.h"
#include "Strategie.h"

namespace Commun {

	Deplacement::Deplacement(Robot& robot, Environment& env, Evitement& evitement)
	        : _robot(robot)
	        , _evitement(evitement)
	        , _carteDeplacement(robot.getCarteDeplacement())
	        , _env(env)
	        , _suiviTrajectoire(1, "Suivi trajectoire") {
		this->pushEvitementState(true);
		this->pushVitesseLineaire(_robot.constantes().getLinearSpeedDefault());
		this->pushVitesseAngulaire(_robot.constantes().getAngularSpeedDefault());
		this->pushPrecisionLineaire(_robot.constantes().getLinearPrecisionDefault());
		this->pushPrecisionAngulaire(_robot.constantes().getAngularPrecisionDefault());

		// Réglage des paramètres de déplacement initiaux
		_carteDeplacement.activerAsservissement(CarteDeplacement::POSITION, true);
		_carteDeplacement.activerAsservissement(CarteDeplacement::ANGLE, true);
		_carteDeplacement.activerAsservissement(CarteDeplacement::DROITE, true);
		_carteDeplacement.activerAsservissement(CarteDeplacement::GAUCHE, true);
		_carteDeplacement.activerEnvoiAuto(true);
	}

	void Deplacement::updateEvitement() {
		// Rien à faire, c'est en local
	}

	void Deplacement::updateVitesseLineaire() {
		_carteDeplacement.reglerParametre(CarteDeplacement::Longitudinal_VitesseMax,
		                                  this->getVitesseLineaire().template toM_s<float>());
	}

	void Deplacement::updateVitesseAngulaire() {
		_carteDeplacement.reglerParametre(CarteDeplacement::Angulaire_VitesseMax,
		                                  this->getVitesseAngulaire().template toRad_s<float>());
	}

	void Deplacement::updatePrecisionLineaire() {
		// La précision côté carte doit être légèrement inférieure à celle côté info
		// sinon on pourrait avoir une précision atteinte côté carte, mais pas côté info -> blocage et fin de
		// déplacement sur timeout uniquement
		_carteDeplacement.reglerParametre(CarteDeplacement::MouvementActuel_PrecisionLongitudinale,
		                                  (this->getPrecisionLineaire()).template toM<float>());
	}

	void Deplacement::updatePrecisionAngulaire() {
		// La précision côté carte doit être légèrement inférieure à celle côté info
		// sinon on pourrait avoir une précision atteinte côté carte, mais pas côté info -> blocage et fin de
		// déplacement sur timeout uniquement
		_carteDeplacement.reglerParametre(CarteDeplacement::MouvementActuel_PrecisionAngulaire,
		                                  (this->getPrecisionAngulaire()).template toRad<float>());
	}

	ResultatAction Deplacement::allerA(Vector2m const& destination, SensAvance sens, Duration timeout) {
		bool shouldIStayOrShouldIGo =
#ifdef HOMOLOGATION
		    false; // le robot ne tente pas les contournements.
#else
		    true; // le robot se chauffe et tente les contournements.
#endif
		// Calcul des coordonnees
		repere::Coordonnees dest(destination, 0_deg, _robot.getStrategie().getReference());

		return allerAInternal(dest, sens, shouldIStayOrShouldIGo, timeout);
	}

	ResultatAction Deplacement::avancerDe(Distance distance, SensAvance sens, Duration timeout) {
		// On attend que la position soit actualisée à coup sûr !
		sleep(_carteDeplacement.getPeriodeEnvoiAuto());

		auto ref = _carteDeplacement.getReference();
		auto posInitiale = _robot.lireCoordonnees().getPos2D(ref);
		auto angleInitial = _robot.lireCoordonnees().getAngle(ref);
		auto now = TimePoint::now();
		// @Benjamin : c'est là que j'suis pas sûr, faut voir comment construire le Vector2d. C'est de la trigo, à toi
		// de
		// voir, en fonction de l'orientation du robot et tout.
		int inverseur = sens == SensAvance::Avant ? 1 : -1;
		auto posAAtteindre = posInitiale + inverseur * distance * Vector2d{cos(angleInitial), sin(angleInitial)};
		ResultatAction res;
		do {
			sleep(_carteDeplacement.getPeriodeEnvoiAuto());
			auto currentPos = _robot.lireCoordonnees().getPos2D(ref);
			auto distance = (posAAtteindre - currentPos).norm();
			logDebug("Current pos: ", currentPos, " | dest = ", posAAtteindre);
			logDebug("Distance restante: ", distance, " & sens: ", sens);
			res = this->avancer(distance, sens, 10_s);
			if(res != ResultatAction::REUSSI) {
				logWarn("Échec de Avancer A ", posAAtteindre, " sens : ", sens, " : ", res);
			}

			if(res == ResultatAction::BLOQUE)
				break;

		} while(res == ResultatAction::BLOQUE_PAR_ADV && (TimePoint::now() - now) <= timeout);

		if((TimePoint::now() - now) > timeout) {
			logWarn("Timeout du allerA.");
		}

		// Notif du timeout
		if(res != ResultatAction::REUSSI) {
			logWarn("Échec de Avancer A ", posAAtteindre, " sens : ", sens, " : ", res);
		}

		logError("FIN DE AVANCER A : STATUT = ", res);
		return res;
	}

	ResultatAction Deplacement::allerAInternal(Coordonnees const& destination, SensAvance sens, bool contourner, Duration timeout) {
		auto deadline = TimePoint::now() + timeout;
		ResultatAction res;

		// On récupère la coordonnees par rapport au robot
		res = this->setTrajectoire(this->calculerTrajectoire(destination, sens, this->getEvitementState()), deadline);
		if(res != ResultatAction::BLOQUE_PAR_ADV) {
			if(res == ResultatAction::REUSSI && _env.isForbidden(destination.getPos2D()))
				return ResultatAction::POSITION_MODIFIEE;
			return res;
		}

		if(contourner) {
			sleep(1_s);
			if(this->_isDebugActive)
				logDebug("Aller A avec contournement !");

			// Mémorise la position actuelle du robot adversaire
			this->_dernierePositionAdversaire = _evitement.getPositionAdversaire();
			this->_advPositionConnue = _evitement.adversaireDetecte();

			// Pour pouvoir se déplacer (ne plus avoir l'adversaire dans la tronche)
			// On tente de reculer.
			SensAvance escapeSens = (sens == SensAvance::Avant) ? SensAvance::Arriere : SensAvance::Avant;
			Distance escapeRadius = this->computeBackupDistance(escapeSens, 20_cm);

			// Si pn ne peut pas reculer assez : on se considère bloqué par adv.
			if(escapeRadius <= 1_cm) {
				return ResultatAction::BLOQUE_PAR_ADV;
			}

			// Si on peut reculer : on le fait.
			res = this->avancer(escapeRadius, escapeSens, timeout);


			if(res != ResultatAction::REUSSI) {
				if(this->_isDebugActive) {
					logDebug("On a pas pu reculer !");
				}
				return res;
			}

			// On retente d'aller à la destination avec le contournement.
			res = this->allerAInternal(destination, sens, true, deadline - TimePoint::now());

			// On supprime les informations de la dernière position de l'adversaire.
			this->_dernierePositionAdversaire = Coordonnees();
			this->_advPositionConnue = false;
		}

		return res;
	}

	Distance Deplacement::computeBackupDistance(SensAvance sens, Distance escapeRadius) {
		Distance escapeStep = 2_cm;
		Coordonnees coords = _robot.actualiserEtLireCoordonnees();
		Angle currentAngle = coords.getAngle();
		Vector2m direction = Vector2m(Distance::makeFromM(cos(currentAngle)), Distance::makeFromM(sin(currentAngle)));

		if(sens == SensAvance::Arriere)
			direction = -direction;

		// On teste plusieurs positions possibles.
		while(escapeRadius > 0_cm) {
			// On recule
			Vector2m escapePosition = coords.getPos2D() + direction * escapeRadius.toM();
			if(!_env.isForbidden(escapePosition)) {
				break;
			}
			escapeRadius -= escapeStep;
		}
		return escapeRadius;
	}

	ResultatAction Deplacement::avancer(Distance distance, SensAvance sens, Duration timeout) {
		if(_evitement.adversairePresent(sens))
			return ResultatAction::BLOQUE_PAR_ADV;

		TimePoint timeoutDate = TimePoint::now() + timeout;
		ResultatAction res = ResultatAction::RATE;
		auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);
		if(actionLock.owns_lock()) {
			if(sens == SensAvance::Avant)
				_carteDeplacement.avancer(distance);
			else
				_carteDeplacement.avancer(-distance);
			res = this->attendreFinTrajectoire(this->testDeplacementUnitaireTermine(), timeoutDate, true, true, sens);
			//_carteDeplacement.arreter();
		} else {
			logDebug3("Carte non disponible");
		}

		// On s'arrête si l'adv nous bloque.
		if(res == ResultatAction::BLOQUE_PAR_ADV) {
			logError("Je m'arrête !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
			this->arretUrgence();
			sleep(100_ms);
		}

		return res;
	}

	ResultatAction Deplacement::avancerInfini(SensAvance sens, Duration timeout) {
		return this->avancer(1000_km, sens, timeout);
	}

	ResultatAction Deplacement::tournerAbsolu(Angle angle, Duration timeout) {
		SensRotation sens =
		    sensRotationOptimal({_robot.lireCoordonnees().getAngle()}, {angle, _robot.getStrategie().getReference()});
		return this->tournerAbsoluOriente(angle, sens, timeout);
	}

	SensRotation Deplacement::sensRotationOptimal(Orientation from, Orientation to) {
		return (to.getAngle() - from.getAngle()).toMinusPiPi() >= 0_rad ? SensRotation::Trigo : SensRotation::Horaire;
	}

	ResultatAction Deplacement::tournerAbsoluOriente(Angle angle, SensRotation sens, Duration timeout) {
		angle = angle.toMinusPiPi();
		TimePoint timeoutDate = TimePoint::now() + timeout;
		auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);
		if(actionLock.owns_lock()) {
			// Expression dans le repère du robot (TODO à faire remonter plus haut et anglifier)
			Coordonnees coordRobot({0_m, 0_m}, angle, _robot.getStrategie().getReference());

			// passage à la carte déplacement
			_carteDeplacement.tournerAbsolu(coordRobot.getAngle(), sens);
			return this->attendreFinTrajectoire(this->testDeplacementUnitaireTermine(), timeoutDate, true, false);
		}
		return ResultatAction::RATE;
	}

	ResultatAction Deplacement::tournerRelatif(Angle angle, Duration timeout) {
		TimePoint timeoutDate = TimePoint::now() + timeout;
		ResultatAction res = ResultatAction::RATE;
		auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);

		if(actionLock.owns_lock()) {
			// changement de repère
			Angle carteAngle = _carteDeplacement.getReference().convertDeltaAngle(_robot.getStrategie().getReference(), angle);

			// passage à la carte déplacement
			_carteDeplacement.tourner(carteAngle);
			res = this->attendreFinTrajectoire(this->testDeplacementUnitaireTermine(), timeoutDate);
		}
		return res;
	}

	ResultatAction Deplacement::pointerVers(Distance x, Distance y, Duration timeout) {
		return pointerVers(x, y, sensRotationOptimal(_robot.lireCoordonnees().getAngle(), Vector2m(x, y).angle()), timeout);
	}

	ResultatAction Deplacement::pointerVers(Distance x, Distance y, SensRotation sens, Duration timeout) {
		TimePoint timeoutDate = TimePoint::now() + timeout;
		ResultatAction res = ResultatAction::RATE;
		auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);

		if(actionLock.owns_lock()) {
			Coordonnees coordsRobot(Vector2m(x, y), 0_deg, _robot.getStrategie().getReference());
			_carteDeplacement.pointerVers(coordsRobot, sens);
			res = this->attendreFinTrajectoire(this->testDeplacementUnitaireTermine(), timeoutDate);
		}
		return res;
	}

	ResultatAction Deplacement::tournerInfini(SensRotation sens, Duration timeout) {
		TimePoint timeoutDate = TimePoint::now() + timeout;
		auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);

		_carteDeplacement.tournerInfini(this->getVitesseAngulaire(), sens);

		size_t compteurBlocages = 0;
		while(TimePoint::now() < timeoutDate) {
			if(verifierBlocage(compteurBlocages, _robot.constantesCommunes().getNombreBlocagePhysiqueMax())) {
				_carteDeplacement.arretUrgence();
				return ResultatAction::BLOQUE;
			}
		}
		_carteDeplacement.arreter();

		return ResultatAction::REUSSI;
	}

	Angle Deplacement::deltaCap(Vector2m const& cible) {
		return (cible - _robot.lireCoordonnees().getPos2D()).angle();
	}

	void Deplacement::arreter() {
		this->setTrajectoire(Trajectoire(), TimePoint::distantFuture());
	}

	void Deplacement::arretUrgence() {
		_carteDeplacement.arretUrgence();

		auto actionLock = this->getLockForAction(CarteAction::Action);
		auto lectureLock = this->getLockForAction(CarteAction::Lecture);

		_carteDeplacement.arretUrgence();

		this->setTrajectoire(Trajectoire(), TimePoint::distantFuture());
	}

	// TODO [impl_repere] this is deprecated, remove this
	void Deplacement::setRepere(Vector2m const& origine, Angle angle) {
		Coordonnees coordsCarte(origine, angle);
		setRepere(coordsCarte);
	}

	void Deplacement::setRepere(Coordonnees const& newCoords) {
		auto& ref = _carteDeplacement.getReference();
		_carteDeplacement.definirCoordonnees(newCoords);

		_originPosition = newCoords.getPos2D(ref);
		_originAngle = newCoords.getAngle(ref);
		// Nécessaire même si le robot est déjà arrêté, sinon la carte est dans un état inconsistant
		_carteDeplacement.arreter();

		// La carte élec est bête, elle réinitialise aussi les vitesses. Donc on lui renvoie.
		this->updateVitesseLineaire();
		this->updateVitesseAngulaire();
		this->updatePrecisionLineaire();
		this->updatePrecisionAngulaire();
	}

	ResultatAction Deplacement::setTrajectoire(Trajectoire&& trajectoire, TimePoint const& timeoutDate) {
		if(trajectoire.size() == 0) {
			return ResultatAction::RATE;
		}

		// On commence par empêcher les actions sur la carte
		{
			auto actionLock = this->getLockForAction(CarteAction::Action, timeoutDate);
			auto lectureLock = this->getLockForAction(CarteAction::Lecture, timeoutDate);

			// Ensuite on bloque le thread de suivi de trajectoire
			std::unique_lock<std::mutex> lkTrajectoire(_mutexTrajectoire);

			// On modifie la trajectoire
			_trajectoire.swap(trajectoire);

			lkTrajectoire.unlock();
			actionLock.unlock();
			lectureLock.unlock();
			if(_robot.isSimuConnected()) {
				logDebug0("Envoi de la position au simu");
			}
		}

		// On lance le suivi de la trajectoire
		auto task = _suiviTrajectoire.addTask(
		    Petri::make_callable([this, timeoutDate]() { return this->suivreTrajectoire(timeoutDate); }));
		task.waitForCompletion();
		_carteDeplacement.arreter();

		return task.returnValue();
	}

	ResultatAction Deplacement::suivreTrajectoire(TimePoint timeoutDate) {
		bool finTrajectoire;
		do {
			std::unique_lock<std::mutex> lkTrajectoire(_mutexTrajectoire);

			finTrajectoire = _trajectoire.empty();

			if(!finTrajectoire) {
				ComposanteTrajectoire nextPoint = _trajectoire.front();
				_trajectoire.pop_front();
				finTrajectoire = _trajectoire.empty();
				lkTrajectoire.unlock();

				ResultatAction resultat = this->allerATrajectoire(nextPoint, finTrajectoire, timeoutDate);

				// Si une erreur s'est produite, on abandonne
				if(resultat != ResultatAction::REUSSI) {
					logWarn("Echec de suivreTrajectoire : ", resultat);
					return resultat;
				}
			}

		} while(!finTrajectoire);

		return ResultatAction::REUSSI;
	}

	ResultatAction Deplacement::allerATrajectoire(ComposanteTrajectoire const& nextPoint, bool finTrajectoire, TimePoint timeoutDate) {
		logDebug3("Passe ?");
		auto lock = this->tryGetLockForAction(CarteAction::Action);
		// Si on a le lock, c'est que la carte est disponible pour le suivi de trajectoire
		if(lock.owns_lock()) {
			Coordonnees nextPos = nextPoint.coordonnees;

			// repère ?
			if(finTrajectoire) {
				/*_carteDeplacement.allerADecompose(nextPoint.coordonnees.x, nextPoint.coordonnees.y, nextPoint.sens);
				return this->attendreFinTrajectoire(this->testDeplacementUnitaireTermine(), timeoutDate, false, true,
				nextPoint.sens);
				*/
				logDebug("Passer par (LAST) : ", nextPos.getPos2D());
				_carteDeplacement.passerPar(nextPos, this->getVitesseLineaire(), nextPoint.sens);
				return this->attendreFinTrajectoire(
				    this->testDeplacementUnitairePrecisionAtteinte(), timeoutDate, true, true, nextPoint.sens);
			} else {
				if(this->_isDebugActive && false) {
					//_carteDeplacement.allerADecompose(nextPoint.coordonnees.x, nextPoint.coordonnees.y,
					// nextPoint.sens);
				}
				logDebug("Passer par : ", nextPos.getPos2D());
				_carteDeplacement.passerPar(nextPos, this->getVitesseLineaire(), nextPoint.sens);
				return this->attendreFinTrajectoire(
				    this->testDeplacementUnitairePrecisionAtteinte(), timeoutDate, false, true, nextPoint.sens);
			}
		}
		// On n'a pas pu avoir un accès exclusif à la carte -> trajectoire plus valide
		return ResultatAction::RATE;
	}

	ResultatAction Deplacement::attendreFinTrajectoire(std::function<bool()> const& conditionFinTrajectoire,
	                                                   TimePoint timeoutDate,
	                                                   bool arret,
	                                                   bool checkAdversaire,
	                                                   SensAvance sensAvance) {
		size_t comptBlocage = 0;

		while(true) {
			// Timeout
			if(TimePoint::now() >= timeoutDate) {
				return ResultatAction::TIMEOUT;
			}

			// Adversaire
			if(checkAdversaire && _evitement.adversairePresent(sensAvance)) {
				return ResultatAction::BLOQUE_PAR_ADV;
			}

			auto lock = this->tryGetLockForAction(CarteAction::Lecture);
			// On n'a pas pu avoir accès en lecture à la carte -> trajectoire plus valide
			if(!lock.owns_lock()) {
				return ResultatAction::RATE;
			}

			// Blocage
			if(this->verifierBlocage(comptBlocage, _robot.constantesCommunes().getNombreBlocagePhysiqueMax())) {
				return ResultatAction::BLOQUE;
			}

			// Fini
			if(conditionFinTrajectoire()) {
				if(arret) {
					arreter();
					sleep(100_ms);
				}
				return ResultatAction::REUSSI;
			}

			sleep(10_ms);
		}
	}

	bool Deplacement::verifierBlocage(size_t& compteurBlocages, size_t min) {
		if(_carteDeplacement.verifierNouveauBlocage()) {
			++compteurBlocages;

			if(compteurBlocages > min) {
				return true;
			}
		}

		return false;
	}

	std::function<bool()> Deplacement::testDeplacementUnitaireTermine() {
		return [this]() { return _carteDeplacement.verifierDeplacementTermine(); };
	}

	std::function<bool()> Deplacement::testDeplacementUnitairePrecisionAtteinte() {
		return [this]() { return _carteDeplacement.verifierPrecisionAtteinte(); };
	}

	typename Deplacement::Trajectoire
	    Deplacement::calculerTrajectoire(Coordonnees const& destination, SensAvance sens, bool /*suiviAdversaire*/) {
		// On récupère la trajectoire depuis l'environnement
		static int fileIndex = 0;
		Coordonnees origin = this->_robot.lireCoordonnees();

		logDebug3("[allerA] Mes coordonnées de départ sont ", origin.getPos2D());
		logDebug3("[allerA] Mes coordonnées d'arrivée sont ", destination.getPos2D());

		// Une valeur de 1 : l'adv à le même radius que le robot.
		const float advRadiusMultiplier = 0.80f;

		// On récupère la position de l'adversaire.
		Coordonnees advPosition;
		bool advPositionIsKnown = false;
		Distance dst = _env.getRobotRadius();

		if(_evitement.adversairePresent(sens)) {
			advPosition = _evitement.getPositionAdversaire();
			advPositionIsKnown = true;

			if(_isDebugActive)
				logDebug("Adversaire présent (", fileIndex, ") : ", advPosition);
		} else if(_advPositionConnue) {
			advPosition = _dernierePositionAdversaire;
			advPositionIsKnown = true;

			if(_isDebugActive)
				logDebug("Adversaire présent 2 (", fileIndex, ") : ", _dernierePositionAdversaire);
		}


		// TODO mettre la gestion de l'adversaire dans une fonction
		if(advPositionIsKnown) {
			// Si y'a un adversaire : on l'ajoute.
			_env.addDynamicShape(1,
			                     std::make_unique<Circle>(Environment::DANGER_INFINITY,
			                                              dst * advRadiusMultiplier,
			                                              Vector2m(advPosition.getX(), advPosition.getY())));

			/*_env.addDynamicShape(2, std::make_unique<Circle>(
			        Environment::DANGER_INFINITY, dst*advRadiusMultiplier*0.5f,
			        (Vector2m(advPosition.x, advPosition.y) + start.getPosition())*0.40f));*/

		} else {
			// Si y'en a pas : on le supprime.
			_env.removeDynamicShape(1);
			//_env.removeDynamicShape(2);

			if(_isDebugActive)
				logDebug("Adversaire absent (", fileIndex, ")");
		}

		// Récupération de la trajectoire calculée par l'A*
		std::vector<Vector2m> trajectoryAstar =
		    _env.getTrajectory(origin.getPos2D(REFERENCE_ASTAR), destination.getPos2D(REFERENCE_ASTAR), 3_s);

		// Dessin en TGA
		fileIndex++;
		if(this->_isDebugActive) {
			// logDebug("Displaying trajectory for debug");
			// Ça flood le CAN ça, que en simu plz !
			// this->afficherTrajectoire(start.getPosition(), destination, trajectoryAstar);
			auto trajcpy = std::vector<Vector2m>(trajectoryAstar);
			trajcpy.emplace_back(origin.getPos2D(REFERENCE_ASTAR)); // rajoute la position initiale dans le dessin
			_env.saveToTGA(("calcul" + std::to_string(fileIndex) + ".tga").c_str(), trajcpy);
		}

		// On calcule les angles (pour créer la trajectoire).
		Trajectoire trajectory;
		for(int i = (int)trajectoryAstar.size() - 1; i >= 0; i--) {
			Vector2m pos;
			if(i == 0) {
				pos = origin.getPos2D(REFERENCE_ASTAR);
			} else {
				pos = trajectoryAstar[i - 1];
			}

			Vector2m nextPos = trajectoryAstar[i];

			Coordonnees coords(nextPos,
			                   Angle::makeFromValue(std::atan2((nextPos.y - pos.y).toM(), (nextPos.x - pos.x).toM())),
			                   Constantes::REFERENCE_ASTAR);

			ComposanteTrajectoire composante(coords, sens);
			trajectory.push_back(composante);
		}

		// Affichage de la trajectoire
		logDebug4("Trajectoire : ");
		for(auto& coord : trajectory) {
			std::cout << coord.coordonnees.getPos2D() << " -> ";
		}
		std::cout << "fin" << std::endl;

		return trajectory;
	}

	void Deplacement::setDebugState(bool state) {
		this->_isDebugActive = state;
	}

	void Deplacement::afficherTrajectoire(Vector2m /*depart*/, Vector2m /*destination*/, std::vector<Vector2m>& trajectoire) {
		bool astar = false;
		if(!trajectoire.empty()) {
			for(int a = 0; a < (int)trajectoire.size(); a++) {
				logDebug0("Trajectoire : ", trajectoire[a]);
			}
		}
		for(int y = 0; y < _robot.constantesCommunes().getTailleGrille().y; y++) {
			for(int x = 0; x < _robot.constantesCommunes().getTailleGrille().x; x++) {
				auto point = _env.getScale() * Vector2i{x, y};
				// logDebug0("Point : ",point);
				astar = false;
				if(!trajectoire.empty()) {
					for(int a = 0; a < (int)trajectoire.size(); a++) {
						if(trajectoire[a].norm() - point.norm() < 0.01_mm) {
							// logDebug("Trajectory point found");
							astar = true;
							//_robot.setCouleurCase(CarteDebugDessin::COULEUR_BLANC); // blanc
							break;
						}
					}
				}
			}
		}
		if(astar) {
			logDebug("Au moins un point a été affiché");
		} else {
			logDebug("Aucun point n'a été affiché");
		}
	}

	Vector2m Deplacement::getOriginPosition() {
		return _originPosition;
	}

	Angle Deplacement::getOriginAngle() {
		return _originAngle;
	}
}

// CarteDeplacement2009.cpp

#include "CarteDeplacement2009.h"
#include "../PhysicalToSimulationUnits.h"
#include "../world/World.h"
#include <cmath>
using std::abs;

constexpr int CarteDeplacement2009::NOMBRE_BLOQUAGE_MAX;
constexpr float CarteDeplacement2009::FACTEUR_VITESSE_LONGITUDINALE;
constexpr float CarteDeplacement2009::FACTEUR_VITESSE_ANGULAIRE;
constexpr Distance CarteDeplacement2009::DISTANCE_INFINIE;
constexpr Distance CarteDeplacement2009::ANGLE_INFINI;

CarteDeplacement2009::CarteDeplacement2009(unsigned char id, RobotLogic& robot, IDeplacement& depla, Distance hauteurTable)
        : Carte(id, robot), _depla(depla), _typeDeplacement(DEPLACEMENT_RIEN) {
	_precisionLongitudinale = 5_cm;
	_precisionAngulaire = 0.05_rad; // rad

	_hauteurTable = hauteurTable;

	reset();
}

void CarteDeplacement2009::reset() {
	_dthetaOrigineTable = 0_rad;
	_dxOrigineTable = 0_cm;
	_dyOrigineTable = 0_cm;

	_deplacementTermineEnvoye = false;
	_deplacementTermine = true;
	_vitesseLongitudinale = 0.0_mm_s;
	_vitesseAngulaire = 0.0_rad_s;
	_etapeDeplacement = 0;
	_angleBrut = 0_rad;
	_angleAAtteindre = 0_rad;
	_distanceTotaleParcourue = 0.0_m;
	_distanceAParcourir = 0.0_m;

	_tempsDernierEnvoi = 0.0_s;
	_tempsActuel = 0.0_s;
	_tempsBloquage = 0;

	_asservissementDistanceActive = true;
	_asservissementAngleActive = true;

	auto pos = getPosition();
	_xPositionPrecedente = pos.x;
	_yPositionPrecedente = pos.y;
	_anglePrecedent = getAngle2d();

	_asservissement.resetPIDs();
}

// Fonction appelée toutes les frames
void CarteDeplacement2009::update(Duration elapsed) {
	Carte::update(elapsed);

	// Envoi de la position toutes les 50 ms
	_tempsActuel += elapsed;
	if(_tempsActuel - _tempsDernierEnvoi > 0.05_s) {
		_tempsDernierEnvoi = _tempsActuel;
		envoyerCoordonnees();
	}

	Distance x, y;

	// Calcul de la distanceTotaleParcourue
	auto pos = getPosition();
	Distance xPositionActuelle = pos.x;
	Distance yPositionActuelle = pos.y;
	x = xPositionActuelle - _xPositionPrecedente;
	y = yPositionActuelle - _yPositionPrecedente;
	Distance distanceSupplementaire = sqrt(x * x + y * y);

	if(_depla.getLinearVelocity() >= 0_m_s)
		_distanceTotaleParcourue += distanceSupplementaire;
	else
		_distanceTotaleParcourue -= distanceSupplementaire;


	// Calcul de l'angleBrut
	Angle angleActuel = getAngle2d();
	Angle angleSupplementaire = abs((angleActuel - _anglePrecedent).toMinusPiPi());

	if(_depla.getAngularVelocity() >= 0_rad_s)
		_angleBrut += angleSupplementaire;
	else
		_angleBrut -= angleSupplementaire;

	// Actualisation des consignes distanceAParcourir et angleAAtteindre en fonction du type de déplacement
	switch(_typeDeplacement) {
		// Les types de déplacement ne nécessitant pas d'actualisation des consignes
		case DEPLACEMENT_ARRETER:
		case DEPLACEMENT_AVANCER:
		case DEPLACEMENT_TOURNER:
		case DEPLACEMENT_TOURNER_ABSOLU:
		case DEPLACEMENT_POINTER_VERS:
			if(abs(_distanceAParcourir - _distanceTotaleParcourue) <= _precisionLongitudinale &&
			   abs(_angleAAtteindre - _angleBrut) <= _precisionAngulaire) {
				if(_deplacementTermineEnvoye == 0) {
					envoyerPrecisionAtteinte();
					_deplacementTermineEnvoye = 1;
				}
				if(estImmobile()) {
					_deplacementTermine = true;
					if(_deplacementTermineEnvoye == 1)
						envoyerDeplacementTermine();
					_deplacementTermineEnvoye = 2;
				}
			}
			break;

		case DEPLACEMENT_ARRET_URGENCE:
			if(abs(_depla.getLinearVelocity()) < 0.1_mm_s && abs(_depla.getAngularVelocity()) < 0.001_rad_s) {
				_deplacementTermine = true;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;
			}
			break;

		// Deplacements nécessitant une actualisation de la consigne, ou ayant plusieurs étapes
		case DEPLACEMENT_ALLER_VERS:
			if(_etapeDeplacement == 0) {
				if(abs(_angleAAtteindre - _angleBrut) <= _precisionAngulaire) {
					// On définit les paramétres de la deuxiéme partie du mouvement
					_etapeDeplacement = 1;

					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					_angleAAtteindre = _angleBrut;
				}
			} else if(abs(_distanceAParcourir - _distanceTotaleParcourue) <= _precisionLongitudinale) {
				if(_deplacementTermineEnvoye == 0) {
					envoyerPrecisionAtteinte();
					_deplacementTermineEnvoye = 1;
				}
				if(estImmobile()) {
					_deplacementTermine = true;
					if(_deplacementTermineEnvoye == 1)
						envoyerDeplacementTermine();
					_deplacementTermineEnvoye = 2;
				}
			}
			break;

		case DEPLACEMENT_ALLER_A_DECOMPOSE:
			if(_etapeDeplacement == 0) { // Rotation
				if(abs(_angleAAtteindre - _angleBrut) <= _precisionAngulaire) {
					// On définit les paramétres de la deuxiéme partie du mouvement
					_etapeDeplacement = 1;
					x = _xConsigne - xPositionActuelle;
					y = _yConsigne - yPositionActuelle;
					_distanceDemandee = sqrt(x * x + y * y);
					_angleDemande = atan2(y, x);

					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					if(_sensDeplacement)
						_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
					else
						_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
				}
			} else { // Translation
				// Si le mouvement est fini, alors on n'actualise plus les consignes
				if(abs(_distanceAParcourir - _distanceTotaleParcourue) <= _precisionLongitudinale) {
					if(_deplacementTermineEnvoye == 0) {
						envoyerPrecisionAtteinte();
						_deplacementTermineEnvoye = 1;
					}
					if(estImmobile()) {
						_deplacementTermine = true;
						if(_deplacementTermineEnvoye == 1)
							envoyerDeplacementTermine();
						_deplacementTermineEnvoye = 2;
					}
				} else { // Sinon on recalcule en permanence la distance restante et l'angle au point
					x = _xConsigne - xPositionActuelle;
					y = _yConsigne - yPositionActuelle;
					_distanceDemandee = sqrt(x * x + y * y);
					_angleDemande = atan2(y, x);

					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					if(_sensDeplacement)
						_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
					else
						_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
				}
			}
			break;

		case DEPLACEMENT_ALLER_A:
			// Si le mouvement est fini, alors on n'actualise plus les consignes
			if(abs(_distanceAParcourir - _distanceTotaleParcourue) <= _precisionLongitudinale) {
				if(_deplacementTermineEnvoye == 0) {
					envoyerPrecisionAtteinte();
					_deplacementTermineEnvoye = 1;
				}
				if(estImmobile()) {
					_deplacementTermine = true;
					if(_deplacementTermineEnvoye == 1)
						envoyerDeplacementTermine();
					_deplacementTermineEnvoye = 2;
				}
			} else { // Sinon on recalcule en permanence la distance restante et l'angle au point
				x = _xConsigne - xPositionActuelle;
				y = _yConsigne - yPositionActuelle;
				_distanceDemandee = sqrt(x * x + y * y);

				if(_sensDeplacement)
					_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
				else
					_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

				_angleDemande = atan2(y, x);
				if(_sensDeplacement)
					_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
				else
					_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
			}
			break;

		case DEPLACEMENT_PASSER_PAR:

			// Si le mouvement est fini, alors on n'actualise plus les consignes

			if(abs(_distanceDemandee) < 2.5_cm // Eviter que le robot dépasse et se mette à tourner
			   &&                              // TODO solution plus propre
			   _deplacementTermine == 0) {
				if(_deplacementTermineEnvoye == 0) {
					envoyerPrecisionAtteinte();
					_deplacementTermineEnvoye = 1;
				}

				_angleAAtteindre = _angleBrut;
			} else if(_deplacementTermine == 0) {
				// Sinon on recalcule en permanence la distance restante et l'angle au point
				x = _xConsigne - xPositionActuelle;
				y = _yConsigne - yPositionActuelle;

				Distance oldDistanceDemandee = _distanceDemandee;
				_distanceDemandee = sqrt(x * x + y * y);
				// std::cout << x << " " << y << " " << _distanceDemandee << " | " << xPositionActuelle << " " <<
				// yPositionActuelle << std::endl;
				if(oldDistanceDemandee <= _distanceDemandee) {
					_distanceDemandee = 2 * oldDistanceDemandee - _distanceDemandee;
				}

				if(_sensDeplacement)
					_distanceAParcourir = DISTANCE_INFINIE;
				else
					_distanceAParcourir = -1 * DISTANCE_INFINIE;

				_angleDemande = atan2(y, x);

				if(_sensDeplacement)
					_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
				else
					_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
			}
			break;

		case DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG:
			// Besoin de rien faire ^^
			break;

		// Deplacements non simulés
		case DEPLACEMENT_PIVOTER_DROITE:
		case DEPLACEMENT_PIVOTER_GAUCHE:
		case DEPLACEMENT_COMMANDER_ROUES:
			break;

		default:
			break;
	}

	auto vitesseLineaireMesuree = abs(_depla.getLinearVelocity());
	auto vitesseLineaireCommande = fromSimulationVL(abs((double)(_asservissement.getPIDPositionVersVitesse().getSortie())));
	auto vitesseAngulaireMesuree = abs(_depla.getAngularVelocity());
	auto vitesseAngulaireCommande =
	    fromSimulationVA(abs((double)(_asservissement.getPIDAngleVersVitesseAngulaire().getSortie())));

	// NB : en DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG, on ne teste pas le bloquage
	if(_typeDeplacement != DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG && (!_deplacementTermine) &&
	   ((vitesseLineaireCommande * FACTEUR_VITESSE_LONGITUDINALE > _precisionLongitudinale / 1_s &&
	     vitesseLineaireMesuree * FACTEUR_VITESSE_LONGITUDINALE < _precisionLongitudinale / 1_s) ||
	    (vitesseAngulaireCommande * FACTEUR_VITESSE_ANGULAIRE > _precisionAngulaire / 1_s &&
	     vitesseAngulaireMesuree * FACTEUR_VITESSE_ANGULAIRE < _precisionAngulaire / 1_s))) {
		_tempsBloquage++;
		if(_tempsBloquage > NOMBRE_BLOQUAGE_MAX)
			_tempsBloquage = NOMBRE_BLOQUAGE_MAX;
	} else {
		_tempsBloquage--;
		if(_tempsBloquage < 0)
			_tempsBloquage = 0;
	}

	if(_tempsBloquage == NOMBRE_BLOQUAGE_MAX) {
		bloque();
		_tempsBloquage = 0;
	}

	Speed vitesseConsigneLineaire = _vitesseConsigne.lineaire;

	if(_typeDeplacement != DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG && _typeDeplacement != DEPLACEMENT_ARRET_URGENCE) {
		_asservissement.calculer(elapsed, _distanceAParcourir, _angleAAtteindre, _distanceTotaleParcourue, _angleBrut, &_vitesseLongitudinale, &_vitesseAngulaire);

		// Modulation de la vitesse linéaire selon l'erreur angulaire
		int signe_vitesse = (int)(_vitesseConsigne.lineaire / abs(_vitesseConsigne.lineaire));
		if(_typeDeplacement == DEPLACEMENT_ALLER_A || _typeDeplacement == DEPLACEMENT_PASSER_PAR ||
		   _typeDeplacement == DEPLACEMENT_CONTROLE_TRAJECTOIRE) {
			vitesseConsigneLineaire =
			    _vitesseConsigne.lineaire * (1.0 - abs((_angleBrut - _angleAAtteindre).toMinusPiPi()) / 0.5_PI);
			if(vitesseConsigneLineaire < 0.0_m_s && signe_vitesse > 0)
				vitesseConsigneLineaire = 0.0_m_s;
			if(vitesseConsigneLineaire > 0.0_m_s && signe_vitesse < 0)
				vitesseConsigneLineaire = 0.0_m_s;
		}

		if(!_asservissementDistanceActive)
			_vitesseLongitudinale = 0_m_s;

		if(!_asservissementAngleActive)
			_vitesseAngulaire = 0_rad_s;
	}

	if(_typeDeplacement == DEPLACEMENT_AVANCER_INFINI) {
		_depla.setAngularVelocity(0.0_rad_s);
		_depla.setLinearVelocity(vitesseConsigneLineaire);
	} else if(_typeDeplacement == DEPLACEMENT_PASSER_PAR) {
		_depla.setAngularVelocity(_vitesseAngulaire);
		_depla.setLinearVelocity(vitesseConsigneLineaire);
		// std::cout << "vitesse angulaire : " << _vitesseAngulaire << " et vitesse linéaire : " <<
		// _vitesseConsigne.lineaire << std::endl;
		// std::cout << "angle à atteindre " << _angleAAtteindre.toDeg() << ", angle brut " << _angleBrut.toDeg() << ",
		// vitesse angulaire "<< _vitesseAngulaire << std::endl;
	} else if(_typeDeplacement == DEPLACEMENT_TOURNER_INFINI) {
		_depla.setAngularVelocity(_vitesseConsigne.angulaire);
		_depla.setLinearVelocity(0.0_m_s);
	} else {
		_depla.setAngularVelocity(_vitesseAngulaire);
		_depla.setLinearVelocity(_vitesseLongitudinale);
	}

	_anglePrecedent = angleActuel;
	_xPositionPrecedente = xPositionActuelle;
	_yPositionPrecedente = yPositionActuelle;
}

// Traite les données fournies par l'IA
void CarteDeplacement2009::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// On a recu un ping (cmd == 0x00), on répond pong
		case PING:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				pong();
			break;

		// On nous a demandé les coordonnées du robot (actualiser_coordonnees() dans le code de Guillaume)
		case COORD:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				envoyerCoordonnees();
			break;

		// On nous a demandé si le déplacement est termine (actualiser_deplacement() dans le code de Guillaume)
		case TERMINE:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				envoyerDeplacementTermine();
			break;

		// Pour cmd == 0x07, cela signifie qu'il s'agit d'un ordre de déplacement, spécifié
		// par le 1er octet de données.
		case COMMANDE:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x00_b)
				arreter();
			else if(t.getNbDonnees() == 3 && t.getDonnee(0) == 0x01_b) {
				auto distance = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));

				avancer(distance);
			} else if(t.getNbDonnees() == 3 && t.getDonnee(0) == 0x02_b) {
				auto angle = Angle::makeFromMilliRad(t.getDonnee<std::int16_t>(1));

				tourner(angle);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x03_b) {
				auto theta = Angle::makeFromMilliRad(t.getDonnee<std::int16_t>(1));
				Sens sens = Sens(t.getDonnee(3));

				tournerAbsolu(theta, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x04_b) {
				auto x = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto y = Distance::makeFromMm(t.getDonnee<std::int16_t>(3));
				Sens sens = Sens(t.getDonnee(5));

				pointerVers(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x05_b) {
				auto x = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto y = Distance::makeFromMm(t.getDonnee<std::int16_t>(3));
				Sens sens = Sens(t.getDonnee(5));

				allerVers(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x06_b) {
				auto x = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto y = Distance::makeFromMm(t.getDonnee<std::int16_t>(3));
				Sens sens = Sens(t.getDonnee(5));

				allerADecompose(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x07_b) {
				auto x = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto y = Distance::makeFromMm(t.getDonnee<std::int16_t>(3));
				Sens sens = Sens(t.getDonnee(5));

				allerA(x, y, sens);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x08_b)
				avancerInfini(Speed::makeFromMm_s(t.getDonnee<short>(1)), Sens(t.getDonnee(3)));
			else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x09_b)
				tournerInfini(AngularSpeed::makeFromMilliRad_s(t.getDonnee<short>(1)), Sens(t.getDonnee(3)));
			else if(t.getNbDonnees() == 8 && t.getDonnee(0) == 0x0A_b) {
				auto x = Distance::makeFromMm(t.getDonnee<short>(1));
				auto y = Distance::makeFromMm(t.getDonnee<short>(3));
				auto vitesse = Speed::makeFromMm_s(t.getDonnee<short>(5));
				Sens sens = Sens(t.getDonnee(7));

				passerPar(x, y, vitesse, sens);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x0B_b) {
				auto angle = Angle::makeFromMilliRad(t.getDonnee<std::int16_t>(1));
				Sens sens = Sens(t.getDonnee(3));

				pivoterDroite(angle, sens);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x0C_b) {
				auto angle = Angle::makeFromMilliRad(t.getDonnee<std::int16_t>(1));
				Sens sens = Sens(t.getDonnee(3));

				pivoterGauche(angle, sens);
			} else if(t.getNbDonnees() == 5 && t.getDonnee(0) == 0x0D_b) {
				auto droite = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto gauche = Distance::makeFromMm(t.getDonnee<std::int16_t>(3));

				commanderRoues(droite, gauche);
			} else if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x0F_b) {
				arretUrgence();
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x10_b) {
				auto distance = Distance::makeFromMm(t.getDonnee<std::int16_t>(1));
				auto angle = Angle::makeFromMilliRad(t.getDonnee<std::int16_t>(3));
				Sens sens = Sens(t.getDonnee(5));

				controleTrajectoire(distance, angle, sens);
			} else if(t.getNbDonnees() == 5 && t.getDonnee(0) == 0x11_b) {
				auto vitesseLongitudinale = 1.0 * Speed::makeFromMm_s(t.getDonnee<std::int16_t>(1));
				auto vitesseAngulaire = 1.0 * AngularSpeed::makeFromMilliRad_s(t.getDonnee<std::int16_t>(3));

				controleVitesseLongAng(vitesseLongitudinale, vitesseAngulaire);
			}
			break;

		case LED:
			if(t.getNbDonnees() == 2) {
				reglerTemoin(bool(t.getDonnee(0)), bool(t.getDonnee(1)));
			}
			break;

		// Demande de mesures
		case MESURE:
			if(type_mesure(t.getDonnee(0)) == MESURE_DISTANCE || type_mesure(t.getDonnee(0)) == MESURE_DROITE ||
			   type_mesure(t.getDonnee(0)) == MESURE_GAUCHE)
				envoyerMesurePosition();
			else if(type_mesure(t.getDonnee(0)) == MESURE_ANGLE)
				envoyerMesureAngle();

		// Reglage des coordonnees actuels
		case DEFINIR_COORD:
			if(t.getNbDonnees() == 6) {
				auto x = Distance::makeFromMm(t.getDonnee<int16_t>(0));
				auto y = Distance::makeFromMm(t.getDonnee<int16_t>(2));
				auto angle = Angle::makeFromMilliRad(t.getDonnee<int16_t>(4));
				setRepere(x, y, angle);
			}
			break;

		// Activation des correcteurs
		case ASSERV_ON_OFF:
			if(t.getNbDonnees() == 2) {
				if(type_grandeur(t.getDonnee(0)) == POSITION)
					_asservissementDistanceActive = bool(t.getDonnee(1));
				else if(type_grandeur(t.getDonnee(0)) == ANGLE)
					_asservissementAngleActive = bool(t.getDonnee(1));
			}
			break;

		// Arrêt d'urgence
		case ARRETURGENCE:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				arretUrgence();
			break;

		case SET_VARIABLE:
			// On ne gere que les variables utiles au simu
			switch(t.getDonnee(0)) {
				case 35_b:
					_asservissement.setVdMax(t.getDonnee<float>(1) * 10.0);
					std::cout << "Vd max :" << _asservissement.getVdMax() << "dm/s" << std::endl;
					break;

				case 43_b:
					_asservissement.setVaMax(t.getDonnee<float>(1));
					std::cout << "Va max :" << _asservissement.getVaMax() << "rad/s" << std::endl;
					break;

				case 207_b:
					_precisionLongitudinale = Distance::makeFromMm(t.getDonnee<float>(1));
					std::cout << "Précision longitudinale :" << _precisionLongitudinale << std::endl;
					break;

				case 208_b:
					_precisionAngulaire = Angle::makeFromRad(t.getDonnee<float>(1));
					std::cout << "Précision angulaire :" << _precisionAngulaire << std::endl;
					break;


				case 39_b:
				case 216_b:
					_asservissement.setAdMax(t.getDonnee<float>(1) * 10.0);
					std::cout << "Ad max :" << _asservissement.getAdMax() << "dm/s2" << std::endl;
					break;

				case 47_b:
				case 217_b:
					_asservissement.setAaMax(t.getDonnee<float>(1));
					std::cout << "Aa max :" << _asservissement.getAaMax() << "rad/s2" << std::endl;
					break;

				default:
					break;
			}
			break;

		case GET_VARIABLE:
			envoyerVariable(t.getDonnee(0)); // Envoi 0.0 car osef de ca dans le simu
			break;

		case ENVOI_AUTO:
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Tous les messages que la carte peut envoyer à l'IA

void CarteDeplacement2009::pong() {
	Trame aEnvoyer(this->getId(), PING);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	logInfo("ping -> pong");
}

void CarteDeplacement2009::envoyerMessageIgnore() {
	Trame aEnvoyer(this->getId(), ACK);
	aEnvoyer.addDonnees('I'); // I pour Ignore (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::envoyerMessageExecute() {
	Trame aEnvoyer(this->getId(), ACK);
	aEnvoyer.addDonnees('R'); // R pour Recu (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::bloque() {
	Trame aEnvoyer(this->getId(), BLOQUE);
	aEnvoyer.addDonnees(1_b); // I pour Ignore (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::envoyerCoordonnees() {
	Trame aEnvoyer(this->getId(), COORD);

	auto pos = getPosition();
	aEnvoyer.addDonnees(pos.x.toMm<short>(), pos.y.toMm<short>(), this->getAngle2d().toMilliRad<short>());

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::envoyerMesurePosition() {
	Trame aEnvoyer(this->getId(), MESURE);

	// TODO : envoyer les bonnes coordonnées !
	aEnvoyer.addByte(MESURE_DISTANCE);
	aEnvoyer.addDonnees(_distanceTotaleParcourue.toMm<std::int16_t>(),
	                    _vitesseLongitudinale.toMm_s<std::int16_t>(),
	                    _vitesseLongitudinale.toMm_s<std::int16_t>());

	std::cout << "Envoi mesures relatives à la position..." << std::endl;

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::envoyerMesureAngle() {
	Trame aEnvoyer(this->getId(), MESURE);

	// TODO : envoyer les bonnes coordonnées !
	aEnvoyer.addByte(MESURE_ANGLE);
	aEnvoyer.addDonnees(_angleBrut.toMilliRad<std::int16_t>(),
	                    _vitesseAngulaire.toMilliRad_s<std::int16_t>(),
	                    _vitesseAngulaire.toMilliRad_s<std::int16_t>());

	std::cout << "Envoi mesures relatives à l'angle..." << std::endl;

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2009::envoyerPrecisionAtteinte() {
	Trame aEnvoyer(this->getId(), TERMINE);
	aEnvoyer.addByte(2);
	this->envoyer(aEnvoyer);

	std::cout << "Envoi precision atteinte..." << std::endl;
}

void CarteDeplacement2009::envoyerDeplacementTermine() {
	Trame aEnvoyer(this->getId(), TERMINE);
	aEnvoyer.addDonnees(_deplacementTermine);
	this->envoyer(aEnvoyer);

	std::cout << "Envoi deplacement terminé..." << std::endl;
}

void CarteDeplacement2009::envoyerVariable(int numero) {
	Trame aEnvoyer(this->getId(), GET_VARIABLE);
	aEnvoyer.addDonnees(numero, 0.0f);
	this->envoyer(aEnvoyer);
}

// Toutes les actions effectuées par la carte ; ces fonctions sont appelées par traiterTrame().

void CarteDeplacement2009::arreter() {
	std::cout << "arrêter" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_ARRETER;
}

void CarteDeplacement2009::avancer(Distance distance) {
	std::cout << "avancer de " << distance << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_distanceAParcourir = distance + _distanceTotaleParcourue;

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_AVANCER;
}

void CarteDeplacement2009::tourner(Angle angle) {
	std::cout << "tourner de " << angle << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	_angleAAtteindre = angle + _angleBrut;

	_typeDeplacement = DEPLACEMENT_TOURNER;
}

void CarteDeplacement2009::tournerAbsolu(Angle theta, Sens sens) {
	std::cout << "se mettre a l'angle " << theta << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_sensDeplacement = sens;

	_distanceAParcourir = _distanceTotaleParcourue;

	_angleAAtteindre = _angleBrut + (theta - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_TOURNER_ABSOLU;
}

// Se tourne vers le point donné
void CarteDeplacement2009::pointerVers(Distance x, Distance y, Sens sens) {
	std::cout << "pointer vers (" << x << "," << y << ")" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_sensDeplacement = sens;

	_distanceAParcourir = _distanceTotaleParcourue;

	_xConsigne = x;
	_yConsigne = y;

	auto pos = getPosition();
	_angleDemande = atan2(_yConsigne - pos.y, _xConsigne - pos.x);

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_POINTER_VERS;
}

// Se rend au point donné par une composition de rotation et d'avance, imprécis => déprécié
void CarteDeplacement2009::allerVers(Distance x, Distance y, Sens sens) {
	std::cout << "aller vers (" << x << "," << y << ")" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_xConsigne = x;
	_yConsigne = y;

	auto pos = getPosition();
	auto tx = _xConsigne - pos.x;
	auto ty = _yConsigne - pos.y;
	_distanceDemandee = sqrt(tx * tx + ty * ty);

	_angleDemande = atan2(_yConsigne - pos.y, _xConsigne - pos.x);

	_sensDeplacement = sens;

	// Premiére étape du mouvement
	_etapeDeplacement = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_ALLER_VERS;
}

// Se rend au point donné par une composition de rotation et d'avance
void CarteDeplacement2009::allerADecompose(Distance x, Distance y, Sens sens) {
	std::cout << "aller à décomposé (" << x << "," << y << ")" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_xConsigne = x;
	_yConsigne = y;

	auto pos = getPosition();
	auto tx = _xConsigne - pos.x;
	auto ty = _yConsigne - pos.y;
	_distanceDemandee = sqrt(tx * tx + ty * ty);

	_angleDemande = atan2(ty, tx);

	_sensDeplacement = sens;

	// Premiére étape du mouvement
	_etapeDeplacement = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_ALLER_A_DECOMPOSE;
}

// Se rend au point donné par une trajectoire lissée
void CarteDeplacement2009::allerA(Distance x, Distance y, Sens sens) {
	std::cout << "aller à (" << x << "," << y << ")" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_sensDeplacement = sens;

	_xConsigne = x;
	_yConsigne = y;

	auto pos = getPosition();
	auto tx = _xConsigne - pos.x;
	auto ty = _yConsigne - pos.y;
	_distanceDemandee = sqrt(tx * tx + ty * ty);
	if(_sensDeplacement)
		_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
	else
		_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

	_angleDemande = atan2(_yConsigne - pos.y, _xConsigne - pos.x);
	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_ALLER_A;
}

// Avance à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2009::avancerInfini(Speed vitesse, Sens sens) {
	std::cout << "avancer infini à " << vitesse << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	if(sens == SENS_AVANT)
		_vitesseConsigne.lineaire = vitesse;
	else
		_vitesseConsigne.lineaire = -vitesse;

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_AVANCER_INFINI;
}

void CarteDeplacement2009::tournerInfini(AngularSpeed vitesse, Sens sens) {
	std::cout << "tourner infini à" << vitesse << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	if(sens == SENS_TRIGO) {
		_distanceAParcourir = _distanceTotaleParcourue;

		_vitesseConsigne.angulaire = vitesse;
	} else {
		_distanceAParcourir = _distanceTotaleParcourue;

		_vitesseConsigne.angulaire = -vitesse;
	}

	_typeDeplacement = DEPLACEMENT_TOURNER_INFINI;
}

// Se rend au point donné par une trajectoire lissée sans s'y arrêter
void CarteDeplacement2009::passerPar(Distance x, Distance y, Speed vitesse, Sens sens) {
	std::cout << "passer par (" << x << "," << y << ")" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_sensDeplacement = sens;

	_xConsigne = x;
	_yConsigne = y;

	auto pos = getPosition();
	std::cout << "ma position actuelle est " << pos << ", angle : " << _angleBrut.toDeg() << std::endl;

	auto tx = _xConsigne - pos.x;
	auto ty = _yConsigne - pos.y;
	_distanceDemandee = sqrt(tx * tx + ty * ty);
	if(_sensDeplacement)
		_vitesseConsigne.lineaire = vitesse;
	else
		_vitesseConsigne.lineaire = -vitesse;

	_angleDemande = atan2(_yConsigne - pos.y, _xConsigne - pos.x);

	std::cout << "L'angle initial vers la cible est " << _angleDemande.toDeg() << std::endl;
	std::cout << "Différence entre l'angle box2D et l'angle Carte : " << _angleBrut.toDeg() << " et "
	          << _depla.getAngle().toDeg() << std::endl;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_PASSER_PAR;
}

// Pivote de l'angle donné autour de la roue droite
void CarteDeplacement2009::pivoterDroite(Angle, Sens) {
	std::cout << "pivoter droite => non geré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_PIVOTER_DROITE;
}

void CarteDeplacement2009::pivoterGauche(Angle, Sens) {
	std::cout << "pivoter gauche => non géré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_PIVOTER_GAUCHE;
}

// Fait avancer chacune des 2 roues d'une distance donnée
void CarteDeplacement2009::commanderRoues(Distance, Distance) {
	std::cout << "commander roues => non géré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_COMMANDER_ROUES;
}

// Arrête immédiatement le déplacement en cours et désactive l'asservissement
void CarteDeplacement2009::arretUrgence() {
	std::cout << "arrêt d'urgence" << std::endl;

	_vitesseLongitudinale = 0.0_m_s;
	_vitesseAngulaire = 0.0_rad_s;
	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_typeDeplacement = DEPLACEMENT_ARRET_URGENCE;
}

// Se rend au point donné par une trajectoire lissée sans s'y arrêter
void CarteDeplacement2009::controleTrajectoire(Distance distance, Angle angle, Sens sens) {
	std::cout << "contrôle trajectoire (" << distance << "," << angle << ") | Sens : " << sens << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = 0;

	_sensDeplacement = sens;

	_distanceDemandee = distance;
	if(_sensDeplacement)
		_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
	else
		_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

	_angleDemande = angle;
	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + (_angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();
	else
		_angleAAtteindre = _angleBrut + (1_PI + _angleDemande - _angleBrut.toMinusPiPi()).toMinusPiPi();

	_typeDeplacement = DEPLACEMENT_CONTROLE_TRAJECTOIRE;
}

void CarteDeplacement2009::controleVitesseLongAng(Speed vitesseLongitudinale, AngularSpeed vitesseAngulaire) {
	// On fixe juste les valeurs des vitesses, ajustées par un coefficient arbitraire
	_vitesseLongitudinale = vitesseLongitudinale;
	_vitesseAngulaire = vitesseAngulaire;

	// On indique le type de déplacement en cours et que le déplacement n'est pas terminé (il ne le sera
	// pas tant qu'on reste en DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG)
	_typeDeplacement = DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG;

	_deplacementTermine = false;
	_deplacementTermineEnvoye = 0;

	_distanceAParcourir = 0_mm;
	_angleAAtteindre = 0.0_rad; // On n'utilise pas ces variables, qui sont mises à 0
}

// Allume ou éteint les LED témoins
void CarteDeplacement2009::reglerTemoin(bool, bool) {}

bool CarteDeplacement2009::estImmobile() {
	return abs(_vitesseLongitudinale) < 0.8_mm_s;
}

// accéder à la position du robot
Vector2m CarteDeplacement2009::getPosition() const {
	auto pos = _depla.getPosition();
	return {pos.x - _dxOrigineTable, pos.y - _dyOrigineTable};
}

// accéder à l'angle du robot vu de dessus
Angle CarteDeplacement2009::getAngle2d() const {
	return (_depla.getAngle() - _dthetaOrigineTable).toMinusPiPi();
}

// modifier le repère du robot
void CarteDeplacement2009::setRepere(Distance x, Distance y, Angle theta) {
	_dxOrigineTable = _dyOrigineTable = 0_m;
	_dthetaOrigineTable = 0_rad;

	auto pos = getPosition();
	_dxOrigineTable = pos.x - x;
	_dyOrigineTable = pos.y - y;
	_dthetaOrigineTable = 0_rad;

	pos = getPosition();

	// réinitialisation de l'asservissement
	_angleBrut = getAngle2d();
	_xPositionPrecedente = pos.x;
	_yPositionPrecedente = pos.y;
	_anglePrecedent = getAngle2d();

	std::cout << "CarteDeplacement2009::setRepere x=" << pos.x << "\ty=" << pos.y << "\ta=" << _angleBrut.toDeg() << std::endl;
}

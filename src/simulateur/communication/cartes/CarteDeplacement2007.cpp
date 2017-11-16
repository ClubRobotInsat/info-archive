// CarteDeplacement2007.cpp

#include "CarteDeplacement2007.h"
#include "btBulletDynamicsCommon.h"

CarteDeplacement2007::CarteDeplacement2007(unsigned char id, Robot& robot, IDeplacement& depla)
        : Carte(id, robot), _depla(depla), _typeDeplacement(DEPLACEMENT_RIEN) {
	_precisionLongitudinale = 0.5; // 5cm
	_precisionAngulaire = 0.1;     // rad
	_deplacementTermineEnvoye = false;
	_deplacementTermine = true;
	_vitesseLongitudinale = 0.0;
	_vitesseAngulaire = 0.0;
	_etapeDeplacement = 0;
	_xSimuToRobot = 0.0;
	_ySimuToRobot = 0.0;
	_thetaSimuToRobot = 0.0;
	_angleBrut = getAngle2d();
	_angleAAtteindre = getAngle2d();
	_distanceTotaleParcourue = 0.0;
	_distanceAParcourir = 0.0;

	_asservissementDistanceActive = true;
	_asservissementAngleActive = true;

	_xPositionPrecedente = getPositionX();
	_yPositionPrecedente = getPositionY();
	_anglePrecedent = getAngle2d();
}

CarteDeplacement2007::~CarteDeplacement2007() {}

// Fonction appelée toutes les frames
void CarteDeplacement2007::update(dure elapsed) {
	Carte::update(elapsed);

	// Envoi de la position toutes les 200 ms
	static double tempsDernierEnvoi = 0.0;
	static double tempsActuel = 0.0;
	tempsActuel += elapsed;
	if(tempsActuel - tempsDernierEnvoi > 0.2) {
		tempsDernierEnvoi = tempsActuel;
		envoyerCoordonnees();
	}

	btScalar x, y;

	// Calcul de la distanceTotaleParcourue
	btScalar xPositionActuelle = getPositionX();
	btScalar yPositionActuelle = getPositionY();
	x = xPositionActuelle - _xPositionPrecedente;
	y = yPositionActuelle - _yPositionPrecedente;
	btScalar distanceSupplementaire = btSqrt(x * x + y * y);

	if(_depla.getLinearVelocity() >= 0.0)
		_distanceTotaleParcourue += distanceSupplementaire;
	else
		_distanceTotaleParcourue -= distanceSupplementaire;

	// Calcul de l'_angleBrut
	btScalar angleActuel = getAngle2d();
	btScalar angleSupplementaire = btFabs(mod2Pi(angleActuel - _anglePrecedent));

	if(_depla.getAngularVelocity() >= 0.0)
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
			if(estDansIntervalle(_distanceAParcourir - _distanceTotaleParcourue, -_precisionLongitudinale, _precisionLongitudinale) &&
			   estDansIntervalle(_angleAAtteindre - _angleBrut, -_precisionAngulaire, _precisionAngulaire)) {
				_deplacementTermine = true;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;
			}
			break;

		case DEPLACEMENT_ARRET_URGENCE:
			if(_depla.getLinearVelocity() == 0.0 && _depla.getAngularVelocity() == 0.0) {
				_deplacementTermine = true;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;
			}
			break;

		// Deplacements nécessitant une actualisation de la consigne, ou ayant plusieurs étapes
		case DEPLACEMENT_ALLER_VERS:
			if(_etapeDeplacement == 0) {
				if(estDansIntervalle(_angleAAtteindre - _angleBrut, -_precisionAngulaire, _precisionAngulaire)) {
					// On définit les paramétres de la deuxiéme partie du mouvement
					_etapeDeplacement = 1;

					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					_angleAAtteindre = _angleBrut;
				}
			} else if(estDansIntervalle(_distanceAParcourir - _distanceTotaleParcourue, -_precisionLongitudinale, _precisionLongitudinale) &&
			          estImmobile()) {
				_deplacementTermine = 1;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;
			}
			break;

		case DEPLACEMENT_ALLER_A_DECOMPOSE:
			if(_etapeDeplacement == 0) { // Rotation
				if(estDansIntervalle(_angleAAtteindre - _angleBrut, -_precisionAngulaire, _precisionAngulaire)) {
					// On définit les paramétres de la deuxiéme partie du mouvement
					_etapeDeplacement = 1;
					x = _yConsigne - yPositionActuelle;
					y = _xConsigne - xPositionActuelle;
					_distanceDemandee = btSqrt(x * x + y * y);
					_angleDemande = btAtan2(_yConsigne - yPositionActuelle, _xConsigne - xPositionActuelle);

					x = _yConsigne - yPositionActuelle;
					y = _xConsigne - xPositionActuelle;
					_distanceDemandee = btSqrt(x * x + y * y);
					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					_angleDemande =
					    btAtan2(_yConsigne - yPositionActuelle,
					            _xConsigne -
					                xPositionActuelle); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?
					if(_sensDeplacement)
						_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
					else
						_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));
				}
			} else { // Translation
				// Si le mouvement est fini, alors on n'actualise plus les consignes
				if(estDansIntervalle(_distanceAParcourir - _distanceTotaleParcourue, -_precisionLongitudinale, _precisionLongitudinale) &&
				   estImmobile()) {
					_deplacementTermine = true;
					if(!_deplacementTermineEnvoye)
						envoyerDeplacementTermine();
					_deplacementTermineEnvoye = true;
				} else { // Sinon on recalcule en permanence la distance restante et l'angle au point
					x = _yConsigne - yPositionActuelle;
					y = _xConsigne - xPositionActuelle;
					_distanceDemandee = btSqrt(x * x + y * y);
					if(_sensDeplacement)
						_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
					else
						_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

					_angleDemande =
					    btAtan2(_yConsigne - yPositionActuelle,
					            _xConsigne -
					                xPositionActuelle); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?
					if(_sensDeplacement)
						_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
					else
						_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));
				}
			}
			break;

		case DEPLACEMENT_ALLER_A:
			// Si le mouvement est fini, alors on n'actualise plus les consignes
			if(estDansIntervalle(_distanceAParcourir - _distanceTotaleParcourue, -_precisionLongitudinale, _precisionLongitudinale) &&
			   estImmobile()) {
				_deplacementTermine = 1;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;
			} else { // Sinon on recalcul en permanence la distance restante et l'angle au point
				x = _yConsigne - yPositionActuelle;
				y = _xConsigne - xPositionActuelle;
				_distanceDemandee = btSqrt(x * x + y * y);
				if(_sensDeplacement)
					_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
				else
					_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

				_angleDemande =
				    btAtan2(_yConsigne - yPositionActuelle,
				            _xConsigne -
				                xPositionActuelle); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?
				if(_sensDeplacement)
					_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
				else
					_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));
			}
			break;

		case DEPLACEMENT_PASSER_PAR:
			// Si le mouvement est fini, alors on n'actualise plus les consignes
			if(estDansIntervalle(_distanceDemandee, -_precisionLongitudinale, _precisionLongitudinale) && _deplacementTermine == 0) {
				_deplacementTermine = true;
				if(!_deplacementTermineEnvoye)
					envoyerDeplacementTermine();
				_deplacementTermineEnvoye = true;

				if(_sensDeplacement)
					_distanceAParcourir = _distanceTotaleParcourue + 5; // Lorsqu'on a atteint le point, on avance
				// encore de 50cm puis on s'arrete, au cas ou le
				// pc n'envoi pas le point suivant.
				else
					_distanceAParcourir = _distanceTotaleParcourue - 5;

				_angleAAtteindre = _angleBrut;
			} else if(_deplacementTermine ==
			          0) { // Sinon on recalcule en permanence la distance restante et l'angle au point
				x = _yConsigne - yPositionActuelle;
				y = _xConsigne - xPositionActuelle;
				_distanceDemandee = btSqrt(x * x + y * y);

				if(_sensDeplacement)
					_distanceAParcourir = DISTANCE_INFINIE;
				else
					_distanceAParcourir = -1 * DISTANCE_INFINIE;

				_angleDemande =
				    btAtan2(_yConsigne - yPositionActuelle,
				            _xConsigne -
				                xPositionActuelle); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?
				if(_sensDeplacement)
					_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
				else
					_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));
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

	btScalar vitesseLineaireMesuree = btFabs(_depla.getLinearVelocity());
	btScalar vitesseLineaireCommande = btFabs((btScalar)(_asservissement.getPIDPositionVersVitesse().getSortie()));
	btScalarMessure = btFabs(_depla.getAngularVelocity());
	btScalarCommande = btFabs((btScalar)(_asservissement.getPIDAngleVersVitesseAngulaire().getSortie()));

	// NB : en DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG, on ne teste pas le bloquage
	if(_typeDeplacement != DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG && (!_deplacementTermine) &&
	   ((vitesseLineaireCommande > _precisionLongitudinale && vitesseLineaireMesuree < _precisionLongitudinale) ||
	    (vitesseAngulaireCommande > _precisionAngulaire && Messure < _precisionAngulaire))) {
		_tempsBloquage++;
		if(_tempsBloquage > TEMPS_BLOQUAGE_MAX)
			_tempsBloquage = TEMPS_BLOQUAGE_MAX;
	} else {
		_tempsBloquage--;
		if(_tempsBloquage < 0)
			_tempsBloquage = 0;
	}

	if(_tempsBloquage == TEMPS_BLOQUAGE_MAX) {
		bloque();
		_tempsBloquage = 0;
	}

	if(_typeDeplacement != DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG) {
		_asservissement.calculer(elapsed, _distanceAParcourir, _angleAAtteindre, _distanceTotaleParcourue, _angleBrut, &_vitesseLongitudinale, &_vitesseAngulaire);

		// Modulation de la vitesse linéaire selon l'erreur angulaire
		if(_typeDeplacement == DEPLACEMENT_PASSER_PAR || _typeDeplacement == DEPLACEMENT_CONTROLE_TRAJECTOIRE)
			_vitesseLongitudinale = _vitesseLongitudinale * (1.0 - btFabs(mod2Pi(_angleBrut - _angleAAtteindre)) / M_PI);

		if(_asservissementDistanceActive == false)
			_vitesseLongitudinale = 0;

		if(_asservissementAngleActive == false)
			_vitesseAngulaire = 0;
	}

	_depla.setAngularVelocity(_vitesseAngulaire);
	_depla.setLinearVelocity(_vitesseLongitudinale);

	_anglePrecedent = angleActuel;
	_xPositionPrecedente = xPositionActuelle;
	_yPositionPrecedente = yPositionActuelle;
}

// Traite les données fournies par l'IA
void CarteDeplacement2007::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// On a recu un ping (cmd == 0x00), on répond pong
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				pong();
			break;

		// On nous demande de regler quelques paramétres : on ne gére que les param utiles au simu
		case 0x03_b:
			switch(type_param(t.getDonnee(0))) {
				case PID:
					if(type_grandeur(t.getDonnee(1)) == POSITION) {
						_asservissement.getPIDPositionVersVitesse().setKp((double)t.getDonnee(2));
						_asservissement.getPIDPositionVersVitesse().setKi((double)t.getDonnee(3));
						_asservissement.getPIDPositionVersVitesse().setKd((double)t.getDonnee(4));

						std::cout << "Reglage PID Position : (" << _asservissement.getPIDPositionVersVitesse().getKp()
						          << ", " << _asservissement.getPIDPositionVersVitesse().getKi() << ", "
						          << _asservissement.getPIDPositionVersVitesse().getKd() << ")" << std::endl;
					} else if(type_grandeur(t.getDonnee(1)) == ANGLE) {
						_asservissement.getPIDAngleVersVitesseAngulaire().setKp((double)t.getDonnee(2));
						_asservissement.getPIDAngleVersVitesseAngulaire().setKi((double)t.getDonnee(3));
						_asservissement.getPIDAngleVersVitesseAngulaire().setKd((double)t.getDonnee(4));

						std::cout << "Reglage PID Angle : (" << _asservissement.getPIDAngleVersVitesseAngulaire().getKp()
						          << ", " << _asservissement.getPIDAngleVersVitesseAngulaire().getKi() << ", "
						          << _asservissement.getPIDAngleVersVitesseAngulaire().getKd() << ")" << std::endl;
					}
					break;

				case CINETIQUE:
					if(type_grandeur(t.getDonnee(1)) == POSITION) {
						if(type_cinetique(t.getDonnee(2)) == VITESSE) {
							_asservissement.setVdMax((double)(short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8)) / 100.0);

							std::cout << "Reglage VdMax : " << _asservissement.getVdMax() << std::endl;
						} else if(type_cinetique(t.getDonnee(2)) == ACCELERATION) {
							_asservissement.setAdMax((double)(short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8)) / 100.0);

							std::cout << "Reglage AdMax : " << _asservissement.getAdMax() << std::endl;
						}
					} else if(type_grandeur(t.getDonnee(1)) == ANGLE) {
						if(type_cinetique(t.getDonnee(2)) == VITESSE) {
							_asservissement.setVaMax((double)(short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8)) / 100.0);

							std::cout << "Reglage VaMax : " << _asservissement.getVaMax() << std::endl;
						} else if(type_cinetique(t.getDonnee(2)) == ACCELERATION) {
							_asservissement.setAaMax((double)(short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8)) / 100.0);

							std::cout << "Reglage AaMax : " << _asservissement.getAaMax() << std::endl;
						}
					}
					break;

				case PRECISION:
					if(type_grandeur(t.getDonnee(1)) == POSITION) {
						_precisionLongitudinale = (double)(short(t.getDonnee(2)) + (short(t.getDonnee(3)) << 8)) / 100.0;

						std::cout << "Reglage Precision Longitudinale : " << _precisionLongitudinale << std::endl;
					} else if(type_grandeur(t.getDonnee(1)) == ANGLE) {
						_precisionAngulaire = (double)(short(t.getDonnee(2)) + (short(t.getDonnee(3)) << 8)) / 100.0;

						std::cout << "Reglage Precision Angulaire : " << _precisionAngulaire << std::endl;
					}
					break;

				default:
					break;
			}


		// On nous a demandé les coordonnées du robot (actualiser_coordonnees() dans le code de Guillaume)
		case 0x05_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				envoyerCoordonnees();
			break;

		// On nous a demandé si le déplacement est termine (actualiser_deplacement() dans le code de Guillaume)
		case 0x06_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				envoyerDeplacementTermine();
			break;

		// Pour cmd == 0x07, cela signifie qu'il s'agit d'un ordre de déplacement, spécifié
		// par le 1er octet de données.
		case 0x07_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x00_b)
				arreter();
			else if(t.getNbDonnees() == 3 && t.getDonnee(0) == 0x01_b) {
				short distance = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);

				avancer(distance);
			} else if(t.getNbDonnees() == 3 && t.getDonnee(0) == 0x02_b) {
				short angle = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);

				tourner(angle);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x03_b) {
				short theta = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				Sens sens = Sens(t.getDonnee(3));

				tournerAbsolu(theta, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x04_b) {
				short x = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short y = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				pointerVers(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x05_b) {
				short x = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short y = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				allerVers(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x06_b) {
				short x = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short y = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				allerADecompose(x, y, sens);
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x07_b) {
				short x = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short y = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				allerA(x, y, sens);
			} else if(t.getNbDonnees() == 2 && t.getDonnee(0) == 0x08_b)
				avancerInfini(Sens(t.getDonnee(1)));
			else if(t.getNbDonnees() == 2 && t.getDonnee(0) == 0x09_b)
				tournerInfini(Sens(t.getDonnee(1)));
			else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x0A_b) {
				short x = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short y = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				passerPar(x, y, sens);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x0B_b) {
				short angle = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				Sens sens = Sens(t.getDonnee(3));

				pivoterDroite(angle, sens);
			} else if(t.getNbDonnees() == 4 && t.getDonnee(0) == 0x0C_b) {
				short angle = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				Sens sens = Sens(t.getDonnee(3));

				pivoterGauche(angle, sens);
			} else if(t.getNbDonnees() == 5 && t.getDonnee(0) == 0x0D_b) {
				short droite = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short gauche = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);

				commanderRoues(droite, gauche);
			} else if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x0F_b) {
				arretUrgence();
			} else if(t.getNbDonnees() == 6 && t.getDonnee(0) == 0x10_b) {
				short distance = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short angle = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);
				Sens sens = Sens(t.getDonnee(5));

				controleTrajectoire(distance, angle, sens);
			} else if(t.getNbDonnees() == 5 && t.getDonnee(0) == 0x11_b) {
				short vitesseLongitudinale = short(t.getDonnee(1)) + (short(t.getDonnee(2)) << 8);
				short = short(t.getDonnee(3)) + (short(t.getDonnee(4)) << 8);

				controleVitesseLongAng(vitesseLongitudinale, );
			}
			break;

		case 0x08_b:
			if(t.getNbDonnees() == 2) {
				reglerTemoin(bool(t.getDonnee(0)), bool(t.getDonnee(1)));
			}
			break;

		// Demande de mesures
		case 0x09_b:
			if(type_mesure(t.getDonnee(0)) == MESURE_DISTANCE || type_mesure(t.getDonnee(0)) == MESURE_DROITE ||
			   type_mesure(t.getDonnee(0)) == MESURE_GAUCHE)
				envoyerMesurePosition();
			else if(type_mesure(t.getDonnee(0)) == MESURE_ANGLE)
				envoyerMesureAngle();

		// Reglage des coordonnees actuels
		case 0x0A_b:
			if(t.getNbDonnees() == 6) {
				btScalar x = (btScalar)(t.getDonnee<short>(0)) / 100.0;
				btScalar y = (btScalar)(t.getDonnee<short>(2)) / 100.0;
				btScalar angle = (btScalar)(t.getDonnee<short>(4)) / 1000.0;
				setRepere(x, y, angle);
			}
			break;

		// Activation des correcteurs
		case 0x0B_b:
			if(t.getNbDonnees() == 2) {
				if(type_grandeur(t.getDonnee(0)) == POSITION)
					_asservissementDistanceActive = bool(t.getDonnee(1));

				if(type_grandeur(t.getDonnee(0)) == ANGLE)
					_asservissementAngleActive = bool(t.getDonnee(1));
			}
			break;

		// Arrêt d'urgence
		case 0x0F_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				arretUrgence();
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Tous les messages que la carte peut envoyer à l'IA

void CarteDeplacement2007::pong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteDeplacement2007::pong ping -> pong" << std::endl;
}

void CarteDeplacement2007::envoyerMessageIgnore() {
	Trame aEnvoyer(this->getId(), 0x01_b);
	aEnvoyer.addDonnees('I'); // I pour Ignore (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::envoyerMessageExecute() {
	Trame aEnvoyer(this->getId(), 0x02_b);
	aEnvoyer.addDonnees('R'); // R pour Recu (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::bloque() {
	Trame aEnvoyer(this->getId(), 0x04_b);
	aEnvoyer.addDonnees(1_b); // I pour Ignore (la vrai carte envoi éa)
	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::envoyerCoordonnees() {
	Trame aEnvoyer(this->getId(), 0x05_b);

	// TODO : envoyer les bonnes coordonnées !
	aEnvoyer.addDonnees(short(getPositionX() * 100.0), // Conversion en mm
	                    short(getPositionY() * 100.0), // Conversion en mm
	                    short(getAngle2d() * 1000.0)); // Conversion en mrad

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::envoyerMesurePosition() {
	Trame aEnvoyer(this->getId(), 0x09_b);

	// TODO : envoyer les bonnes coordonnées !
	aEnvoyer.addByte(MESURE_DISTANCE);
	aEnvoyer.addDonnees(short(_distanceTotaleParcourue * 100.0), // Conversion en mm
	                    short(_vitesseLongitudinale * 100.0),    // Conversion en mm/s
	                    short(_vitesseLongitudinale * 100.0));   // Conversion en mm/s

	std::cout << "Envoi mesures relatives a la position..." << std::endl;

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::envoyerMesureAngle() {
	Trame aEnvoyer(this->getId(), 0x09_b);

	// TODO : envoyer les bonnes coordonnées !
	aEnvoyer.addByte(MESURE_ANGLE);
	aEnvoyer.addDonnees(short(_angleBrut * 100.0),         // Conversion en mrad
	                    short(_vitesseAngulaire * 100.0),  // Conversion en mrad/s
	                    short(_vitesseAngulaire * 100.0)); // Conversion en mrad/s

	std::cout << "Envoi mesures relatives a l'angle..." << std::endl;

	this->envoyer(aEnvoyer);
}

void CarteDeplacement2007::envoyerDeplacementTermine() {
	Trame aEnvoyer(this->getId(), 0x06_b);
	aEnvoyer.addDonnees(_deplacementTermine);
	this->envoyer(aEnvoyer);
}

// Toutes les actions effectuées par la carte ; ces fonctions sont appelées par traiterTrame().

void CarteDeplacement2007::arreter() {
	std::cout << "arreter" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_distanceAParcourir = _distanceTotaleParcourue;

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_ARRETER;
}

void CarteDeplacement2007::avancer(short distance) {
	std::cout << "avancer de " << distance << " mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_distanceAParcourir = (btScalar)(distance) / 100.0 + _distanceTotaleParcourue; // Conversion en dm de la consigne

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_AVANCER;
}

void CarteDeplacement2007::tourner(short angle) {
	std::cout << "tourner de " << angle << " mrad" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_distanceAParcourir = _distanceTotaleParcourue;

	_angleAAtteindre = (btScalar)(angle) / 1000.0 + _angleBrut; // Conversion en rad

	_typeDeplacement = DEPLACEMENT_TOURNER;
}

void CarteDeplacement2007::tournerAbsolu(short theta, Sens sens) {
	std::cout << "se mettre a l'angle " << theta << " mrad" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_sensDeplacement = sens;

	_distanceAParcourir = _distanceTotaleParcourue;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi((btScalar)(theta) / 1000.0 - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + (btScalar)(theta) / 1000.0 - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_TOURNER_ABSOLU;
}

// Se tourne vers le point donné (mm, mm)
void CarteDeplacement2007::pointerVers(short x, short y, Sens sens) {
	std::cout << "pointer vers (" << x << "," << y << ") mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_sensDeplacement = sens;

	_distanceAParcourir = _distanceTotaleParcourue;

	_xConsigne = (btScalar)(x) / 100.0;

	_yConsigne = (btScalar)(y) / 100.0;

	_angleDemande = btAtan2(_yConsigne - getPositionY(),
	                        _xConsigne - getPositionX()); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_POINTER_VERS;
}

// Se rend au point donné (mm, mm) par une composition de rotation et d'avance, imprécis => déprécié
void CarteDeplacement2007::allerVers(short x, short y, Sens sens) {
	std::cout << "aller vers (" << x << "," << y << ") mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_xConsigne = x / 100.0;

	_yConsigne = y / 100.0;

	btScalar tx = _yConsigne - getPositionY();
	btScalar ty = _xConsigne - getPositionX();
	_distanceDemandee = btSqrt(tx * tx + ty * ty);

	_angleDemande = btAtan2(_yConsigne - getPositionY(),
	                        _xConsigne - getPositionX()); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?

	_sensDeplacement = sens;

	// Premiére étape du mouvement
	_etapeDeplacement = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_ALLER_VERS;
}

// Se rend au point donné (mm, mm) par une composition de rotation et d'avance
void CarteDeplacement2007::allerADecompose(short x, short y, Sens sens) {
	std::cout << "aller a (decompose) (" << x << "," << y << ") mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_xConsigne = x / 100.0;

	_yConsigne = y / 100.0;

	btScalar tx = _yConsigne - getPositionY();
	btScalar ty = _xConsigne - getPositionX();
	_distanceDemandee = btSqrt(tx * tx + ty * ty);

	_angleDemande = btAtan2(_yConsigne - getPositionY(),
	                        _xConsigne - getPositionX()); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?

	_sensDeplacement = sens;

	// Premiére étape du mouvement
	_etapeDeplacement = 0;

	_distanceAParcourir = _distanceTotaleParcourue;

	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_ALLER_A_DECOMPOSE;
}

// Se rend au point donné (mm, mm) par une trajectoire lissée
void CarteDeplacement2007::allerA(short x, short y, Sens sens) {
	std::cout << "aller a (" << x << "," << y << ") mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_sensDeplacement = sens;

	_xConsigne = x / 100.0;

	_yConsigne = y / 100.0;

	btScalar tx = _yConsigne - getPositionY();
	btScalar ty = _xConsigne - getPositionX();
	_distanceDemandee = sqrt(tx * tx + ty * ty);
	if(_sensDeplacement)
		_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
	else
		_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

	_angleDemande = btAtan2(_yConsigne - getPositionY(),
	                        _xConsigne - getPositionX()); // Est ce que c'est bien Z qui correspond é la coordonnée Y ?
	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_ALLER_A;
}

// Avance à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2007::avancerInfini(Sens sens) {
	std::cout << "avancer infini" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	if(sens == SENS_AVANT)
		_distanceAParcourir = DISTANCE_INFINIE;
	else
		_distanceAParcourir = -1 * DISTANCE_INFINIE;

	_angleAAtteindre = _angleBrut;

	_typeDeplacement = DEPLACEMENT_AVANCER_INFINI;
}

void CarteDeplacement2007::tournerInfini(Sens sens) {
	std::cout << "tourner infini" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	if(sens == SENS_TRIGO) {
		_distanceAParcourir = _distanceTotaleParcourue;

		_angleAAtteindre = ANGLE_INFINI;
	} else {
		_distanceAParcourir = _distanceTotaleParcourue;

		_angleAAtteindre = -1 * ANGLE_INFINI;
	}

	_typeDeplacement = DEPLACEMENT_TOURNER_INFINI;
}

// Se rend au point donné (mm, mm) par une trajectoire lissée sans s'y arrêter
void CarteDeplacement2007::passerPar(short x, short y, Sens sens) {
	std::cout << "passer par (" << x << "," << y << ") mm" << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_sensDeplacement = sens;

	_xConsigne = x / 100.0;

	_yConsigne = y / 100.0;

	btScalar tx = _yConsigne - getPositionY();
	btScalar ty = _xConsigne - getPositionX();
	_distanceDemandee = btSqrt(tx * tx + ty * ty);
	if(_sensDeplacement)
		_distanceAParcourir = DISTANCE_INFINIE;
	else
		_distanceAParcourir = -1 * DISTANCE_INFINIE;

	_angleDemande = btAtan2(_yConsigne - getPositionY(), _xConsigne - getPositionX());
	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_PASSER_PAR;
}

// Pivote de l'angle donné (mrad) autour de la roue droite
void CarteDeplacement2007::pivoterDroite(short angle, Sens sens) {
	std::cout << "pivoter droite => non geré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_PIVOTER_DROITE;
}

void CarteDeplacement2007::pivoterGauche(short angle, Sens sens) {
	std::cout << "pivoter gauche => non geré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_PIVOTER_GAUCHE;
}

// Fait avancer chacune des 2 roues d'une distance donnée (mm, mm)
void CarteDeplacement2007::commanderRoues(short droite, short gauche) {
	std::cout << "commander roues => non geré par le simulateur" << std::endl;

	_typeDeplacement = DEPLACEMENT_COMMANDER_ROUES;
}

// Arrête immédiatement le déplacement en cours et désactive l'asservissement
void CarteDeplacement2007::arretUrgence() {
	std::cout << "arret d'urgence" << std::endl;

	_vitesseLongitudinale = 0.0;
	_vitesseAngulaire = 0.0;
	_deplacementTermine = 0;

	_distanceAParcourir = _distanceTotaleParcourue + _depla.getLinearVelocity() * 0.15;
	_angleAAtteindre = _angleBrut + _depla.getAngularVelocity() * 0.15;

	_deplacementTermineEnvoye = false;

	_typeDeplacement = DEPLACEMENT_ARRET_URGENCE;
}

// Se rend au point donné (mm, mm) par une trajectoire lissée sans s'y arrêter
void CarteDeplacement2007::controleTrajectoire(short distance, short angle, Sens sens) {
	std::cout << "controle trajectoire (" << distance << "," << angle << ") <mm,mrad> | Sens : " << sens << std::endl;

	_deplacementTermine = 0;

	_deplacementTermineEnvoye = false;

	_sensDeplacement = sens;

	_distanceDemandee = (double)(distance) / 100.0;
	if(_sensDeplacement)
		_distanceAParcourir = _distanceTotaleParcourue + _distanceDemandee;
	else
		_distanceAParcourir = _distanceTotaleParcourue - _distanceDemandee;

	_angleDemande = (double)(angle) / 1000.0;
	if(_sensDeplacement)
		_angleAAtteindre = _angleBrut + mod2Pi(_angleDemande - mod2Pi(_angleBrut));
	else
		_angleAAtteindre = _angleBrut + mod2Pi(M_PI + _angleDemande - mod2Pi(_angleBrut));

	_typeDeplacement = DEPLACEMENT_CONTROLE_TRAJECTOIRE;
}

void CarteDeplacement2007::controleVitesseLongAng(short vitesseLongitudinale, short) {
	// On fixe juste les valeurs des vitesses, ajustées par un coefficient arbitraire
	_vitesseLongitudinale = FACTEUR_VITESSE_LONGITUDINALE * btScalar(vitesseLongitudinale);
	_vitesseAngulaire = FACTEUR_VITESSE_ANGULAIRE * btScalar(vitesseAngulaire);

	// On indique le type de déplacement en cours et que le déplacement n'est pas terminé (il ne le sera
	// pas tant qu'on reste en DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG)
	_typeDeplacement = DEPLACEMENT_CONTROLE_VITESSE_LONG_ANG;

	_deplacementTermine = false;
	_deplacementTermineEnvoye = false;

	_distanceAParcourir = _angleAAtteindre = 0.0; // On n'utilise pas ces variables, qui sont mises à 0
}

// Allume ou éteint les LED témoins
void CarteDeplacement2007::reglerTemoin(bool led1, bool led2) {}

btScalar CarteDeplacement2007::mod2Pi(btScalar angle) {
	btScalar resultat = angle;

	while((resultat > M_PI) || (resultat <= -M_PI)) {
		if(resultat > M_PI) {
			resultat -= 2 * M_PI;
		} else if(resultat <= -M_PI) {
			resultat += 2 * M_PI;
		}
	}

	return resultat;
}

bool CarteDeplacement2007::estDansIntervalle(btScalar val, btScalar min, btScalar max) {
	if(val >= min && val <= max)
		return true;
	else
		return false;
}

bool CarteDeplacement2007::estImmobile() {
	// cout << "vitesseAngulaire=" << << " vitesseLongitudinale=" << vitesseLongitudinale << endl;
	if(-0.3 < _vitesseLongitudinale && _vitesseLongitudinale < 0.3)
		return true;
	return false;
}

// fonction qui sont par rapport au repere du robot

// acceder a la position du robot en deciMetre
btScalar CarteDeplacement2007::getPositionX() const {
	return _depla.getPosition().getX();
}

// acceder a la position du robot en deciMetre
btScalar CarteDeplacement2007::getPositionY() const {
	return -_depla.getPosition().getZ();
}

// acceder a l'angle du robot vu de dessus en radian
btScalar CarteDeplacement2007::getAngle2d() const {
	return _depla.getAngle2d() - M_PI / 2;
}

// modifier le repere du robot en deciMetre et radian
void CarteDeplacement2007::setRepere(btScalar x, btScalar y, btScalar theta) {
	_depla.setPositionAngle(btVector3(x, _depla.getPosition().getY(), -y), theta);

	std::cout << "CarteDeplacement2007::setRepere ATTENTION L'ANGLE THETA N'EST PAS GERE PAR CETTE FONCTIONNALITE " << std::endl;
	std::cout << "CarteDeplacement2007::setRepere x=" << x << "\ty=" << y << "\ta=" << theta << std::endl;
	// _depla. // TODO teleporter le robot
	std::cout << "CarteDeplacement2007::setRepere x=" << getPositionX() << "\ty=" << getPositionY()
	          << "\ta=" << getAngle2d() << std::endl;

	// reinitilisation de l'asservissement
	_angleBrut = getAngle2d();
	_xPositionPrecedente = getPositionX();
	_yPositionPrecedente = getPositionY();
	_anglePrecedent = getAngle2d();
}

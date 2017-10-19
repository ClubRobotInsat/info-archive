// CarteAsservissement2009.cpp

#include "CarteAsservissement2009.h"

// Pour le correcteur proportionnel en vitesse :
const Angle CarteAsservissement2009::ASSERVIS_EPSILON = 0.001_rad;
const float CarteAsservissement2009::ASSERVIS_COEFF_VITESSE = 20.f;

const AngularSpeed CarteAsservissement2009::ASSERVIS_VITESSE_MAX = 3_PI / 1_s;
constexpr Duration CarteAsservissement2009::DELTA_T_ENVOI_AUTO;

// IDs des messages ; récupéré du code de l'IE.
enum IDMessage {
	RESET = 15,
	DETECT = 14,
	GET_VARIABLE = 13, // Non implémenté
	MESURE = 12,
	TERMINE = 11,
	ANGLE = 10,
	BLOQUE = 9,
	LED = 8,
	ENVOI_AUTO = 7,
	ASSERV_ON_OFF = 6,
	ARRETURGENCE = 5,
	COMMANDE = 4,
	DEFINIR_ANGLE = 3,
	SET_VARIABLE = 2, // Non implémenté
	ACK = 1,          // Inutilisé ici
	PING = 0
};

// Constructeur
CarteAsservissement2009::CarteAsservissement2009(unsigned char id, RobotLogic& robot, IMoteurAsservi& iMoteurAsservi)
        : Carte(id, robot)
        , _iMoteurAsservi(iMoteurAsservi)
        , _vitesseVoulue(0.0_rad_s)
        , _angleVoulu(0.0_rad)
        , _ordreEnCours(DEPLACEMENT_ARRETER)
        , _decalageAngle(0.0_rad)
        , _asservActive(true)
        , _envoiAutoActive(false)
        , _deplacementTermineEnvoye(false)
        , _precisionAtteinteEnvoye(false) {
	// Angle moteur = Angle IA + Décalage
	// => Décalage = Angle moteur - Angle IA
	// => Décalage = Angle moteur - 0.0
	_decalageAngle = iMoteurAsservi.getEngineAngle(this->getId());
}

// Traite les données fournies par l'IA
void CarteAsservissement2009::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		// Ping / pong :
		case PING:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Lecture de l'angle :
		case ANGLE:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				envoyerAngle();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Redéfinition de l'angle :
		case DEFINIR_ANGLE:
			if(t.getNbDonnees() == 4) {
				definirAngle(Angle::makeFromMilliRad(t.getDonnee<int>(0)));
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Commande :
		case COMMANDE:
			if(t.getNbDonnees() >= 1) {
				switch(t.getDonnee(0)) {
					case DEPLACEMENT_ARRETER:
						if(t.getNbDonnees() == 1)
							arreter();
						else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					case DEPLACEMENT_TOURNER:
						if(t.getNbDonnees() == 5) {
							tourner(Angle::makeFromMilliRad(t.getDonnee<int>(1)));
						} else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					case DEPLACEMENT_TOURNER_ABSOLU:
						if(t.getNbDonnees() == 6) {
							tournerAbsolu(Angle::makeFromMilliRad(t.getDonnee<int>(1)), (SensRotation)(t.getDonnee(5)));
						} else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					case DEPLACEMENT_TOURNER_INFINI:
						if(t.getNbDonnees() == 6) {
							auto vitesse = AngularSpeed::makeFromMilliRad_s(t.getDonnee<int>(1));
							tournerInfini(vitesse, (SensRotation)(t.getDonnee(5)));
							envoyerDeplacementTermine();
						} else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					case DEPLACEMENT_ARRET_URGENCE:
						if(t.getNbDonnees() == 1)
							arretUrgence();
						else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					// Application directe de la tension du moteur (utilisé pour commander le tapis roulant 2009)
					case 5:
						if(t.getNbDonnees() == 2) {
							int tension = (int)(t.getDonnee(1));
							if(tension > 0)
								tournerInfini(2_rad_s, SENS_HORAIRE);
						} else
							throw Trame::ErreurTrameNonTraitee(t);
						break;

					default:
						throw Trame::ErreurTrameNonTraitee(t);
						break;
				}
			} else {
				throw Trame::ErreurTrameNonTraitee(t);
			}
			break;

		// Allumage de la LED
		case LED:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				logInfo("LED");
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Demande d'une mesure
		case MESURE:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0xFF_b)
				logInfo("demande de mesure : non traite");
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Choix du type d'asservissement : angle ou vitesse
		case ASSERV_ON_OFF:
			if(t.getNbDonnees() == 1) {
				_asservActive = (bool)(t.getDonnee(0));
				if(_asservActive)
					logInfo("activation de l'asservissement [id carte=", (int)(this->getId()), "]");
				else
					logInfo("desactivation de l'asservissement [id carte=", (int)(this->getId()), "]");
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Activation / désactivation de l'envoi auto
		case ENVOI_AUTO:
			if(t.getNbDonnees() == 1)
				_envoiAutoActive = (bool)(t.getDonnee(0));
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Changement de paramètre
		case SET_VARIABLE:
			if(t.getNbDonnees() == 5)
				logInfo("SET_VARIABLE : non implemente");
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// Lecture d'un paramètre
		case GET_VARIABLE:
			if(t.getNbDonnees() == 1)
				if(t.getDonnee(0) == 24) { // Etat_AngleBrut
					envoyerParametre(_iMoteurAsservi.getEngineAngle(getId()).toRad<float>());
				} else
					logInfo("GET_VARIABLE : non implemente");
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
			break;
	}
}

////////////////////// Envois de messages /////////////////

void CarteAsservissement2009::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);

	logInfo("ping -> pong");
}

void CarteAsservissement2009::envoyerBloque() {
	Trame aEnvoyer(this->getId(), BLOQUE);
	aEnvoyer.addByte(0xFF_b);
	this->envoyer(aEnvoyer);

	// logInfo("moteur de la carte ", (Byte)this->getId(), " est bloque");
}

void CarteAsservissement2009::envoyerAngle() {
	Trame aEnvoyer(this->getId(), ANGLE);

	// Angle moteur = Angle IA + Décalage
	// => Angle IA = Angle moteur - Décalage
	auto angle = _iMoteurAsservi.getEngineAngle(this->getId()) - _decalageAngle;
	aEnvoyer.addDonnees(angle.toMilliRad<int>());
	this->envoyer(aEnvoyer);
}

void CarteAsservissement2009::envoyerDeplacementTermine() {
	Trame aEnvoyer(this->getId(), TERMINE);
	aEnvoyer.addByte(1); // 1 indique que le déplacement est terminé et que la précision est atteinte
	this->envoyer(aEnvoyer);
}

void CarteAsservissement2009::envoyerPrecisionAtteinte() {
	Trame aEnvoyer(this->getId(), TERMINE);
	aEnvoyer.addByte(2); // 2 indique que le déplacement n'est PAS terminé et que la précision est atteinte
	this->envoyer(aEnvoyer);
}
void CarteAsservissement2009::envoyerParametre(float f) {
	Trame aEnvoyer(this->getId(), GET_VARIABLE);
	aEnvoyer.addDonnees(f);
	this->envoyer(aEnvoyer);
}

////////////////////////// Actions ////////////////////

void CarteAsservissement2009::definirAngle(Angle angle) {
	// Angle moteur = Angle IA + Décalage
	// => Décalage = Angle moteur - Angle IA
	_decalageAngle = _iMoteurAsservi.getEngineAngle(this->getId()) - angle;
}

void CarteAsservissement2009::arreter() {
	_ordreEnCours = DEPLACEMENT_ARRETER;
}

void CarteAsservissement2009::tourner(Angle angle) {
	_angleVoulu += angle;
	_ordreEnCours = DEPLACEMENT_TOURNER;

	_deplacementTermineEnvoye = false;
	_precisionAtteinteEnvoye = false;
}

void CarteAsservissement2009::tournerAbsolu(Angle angle, SensRotation sens) {
	_angleVoulu = angle * (sens == SENS_TRIGO ? 1.0 : -1.0);
	_ordreEnCours = DEPLACEMENT_TOURNER_ABSOLU;
	_deplacementTermineEnvoye = false;
	_precisionAtteinteEnvoye = false;
	logDebug0("tournerAbsolu : ", angle);
}

void CarteAsservissement2009::tournerInfini(AngularSpeed vitesse, SensRotation sens) {
	_vitesseVoulue = vitesse * (sens == SENS_TRIGO ? 1.0 : -1.0) * _iMoteurAsservi.getCoeffConversionAngularLinearSpeed();
	_ordreEnCours = DEPLACEMENT_TOURNER_INFINI;
	_precisionAtteinteEnvoye = false;
}

void CarteAsservissement2009::arretUrgence() {
	_ordreEnCours = DEPLACEMENT_ARRET_URGENCE;
}

////////////////////////// Update //////////////////////
void CarteAsservissement2009::update(Duration elapsed) {
	Carte::update(elapsed);

	if(_iMoteurAsservi.isBlocked(this->getId()))
		envoyerBloque();

	// Si pas d'asservissement : on passe
	if(!_asservActive)
		return;

	// On se met direct à la vitesse voulue (pas d'asservissement ^^)
	if(_ordreEnCours == DEPLACEMENT_ARRETER || _ordreEnCours == DEPLACEMENT_ARRET_URGENCE) {
		_iMoteurAsservi.setEngineAngularSpeed(this->getId(), 0.0_rad_s);
	} else if(_ordreEnCours == DEPLACEMENT_TOURNER || _ordreEnCours == DEPLACEMENT_TOURNER_ABSOLU) {
		Angle angleActuel = _iMoteurAsservi.getEngineAngle(this->getId());

		// Si l'on est assez précis : on envoi le message "précision atteinte" s'il n'a pas
		// encore été envoyé, et on arrête de faire tourner le moteur
		if(abs(angleActuel - _angleVoulu) < ASSERVIS_EPSILON) {
			_vitesseVoulue = 0.0_rad_s;

			if(!_precisionAtteinteEnvoye) {
				envoyerPrecisionAtteinte();
				envoyerDeplacementTermine();
			}

			_precisionAtteinteEnvoye = true;
		}

		// Si l'on n'est pas assez près de l'angle voulu :
		else {
			// On calcule une vitesse de commande :
			_vitesseVoulue = (_angleVoulu - angleActuel) * ASSERVIS_COEFF_VITESSE / 1_s;
			if(_vitesseVoulue > ASSERVIS_VITESSE_MAX)
				_vitesseVoulue = ASSERVIS_VITESSE_MAX;
			else if(_vitesseVoulue < -ASSERVIS_VITESSE_MAX)
				_vitesseVoulue = -ASSERVIS_VITESSE_MAX;
			// logDebug0("vitesse voulue : ", _vitesseVoulue);
		}

		// On envoie la vitesse calculée précédemment au moteur
		_iMoteurAsservi.setEngineAngularSpeed(this->getId(), _vitesseVoulue);
	} else if(_ordreEnCours == DEPLACEMENT_TOURNER_INFINI) {
		_iMoteurAsservi.setEngineAngularSpeed(this->getId(), _vitesseVoulue);
	}
	// logDebug0("vitesse voulue : ", _vitesseVoulue);
	// Envoi auto : on envoie
	if(this->_envoiAutoActive) {
		if(_horloge.getElapsedTime() >= DELTA_T_ENVOI_AUTO) {
			envoyerAngle();
			_horloge.reset();
		}
	}
}

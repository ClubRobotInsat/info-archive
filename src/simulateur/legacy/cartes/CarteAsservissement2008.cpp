// CarteAsservissement2008.cpp

#include "CarteAsservissement2008.h"

const AngularSpeed CarteAsservissement2008::ASSERVIS_VITESSE_MAX = 4_PI / 1_s;
const Angle CarteAsservissement2008::ASSERVIS_EPSILON = 0.01_rad;

// constructeur
CarteAsservissement2008::CarteAsservissement2008(unsigned char id, Robot& robot, IMoteurAsservi& iMoteurAsservi)
        : Carte(id, robot), _iMoteurAsservi(iMoteurAsservi), _decalageAngle(0_rad) {
	// faire en sorte qu'au demarrage l'angle sois toujours null
	// decalageAngle = iMoteurAsservi.getAngleMoteur(getId()); TODO une fois le chagement d'angle teste
}

// destructeur
CarteAsservissement2008::~CarteAsservissement2008() {}

// Traite les données fournies par l'IA
void CarteAsservissement2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// ping => repondre un pong
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b)
				envoyerPong();
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// commande en vitesse
		case 0x01_b:
			if(t.getNbDonnees() == 2) {
				// acceder au donnee
				_vitesse = AngularSpeed::makeFromMilliRad_s(t.getDonnee<short>(0));
				// mode asservissement vitesse (le update ne la modifira plus)
				_typeAsservis = ASSERVIS_VITESSE;
				// modifier la vitesse du moteur
				_iMoteurAsservi.setVitesseAngulaireMoteur(getId(), _vitesse);
				// envoyer l'acquittement
				envoyerVitesseAtteint();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		// commande en angle
		case 0x02_b:
			if(t.getNbDonnees() == 2) {
				// acceder au donnee
				_angle = Angle::makeFromMilliRad(t.getDonnee<short>(0));
				_angle += _decalageAngle;
				_angle = -_angle;
				_angle = _angle.toMinusPiPi();
				// mode asservissement position (la modification sera effectuer par le update)
				_typeAsservis = ASSERVIS_POSITION;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// stop
		case 0x03_b:
			if(t.getNbDonnees() == 1) {
				// mode asservissement vitesse (le update ne la modifira plus)
				_typeAsservis = ASSERVIS_VITESSE;
				_vitesse = 0.0_rad_s;
				// modifier la vitesse du moteur
				_iMoteurAsservi.setVitesseAngulaireMoteur(getId(), _vitesse);
				// envoyer l'acquittement
				envoyerPositionAtteint();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// redefinir la valeur de l'angle pour la position courante (en mrad)
		case 0x06_b:
			if(t.getNbDonnees() == 2) {
				// TODO a tester
				std::cout << "asservissement : reinitialiser position : " << t.getDonnee<short>(0) << std::endl;
				_decalageAngle = -_iMoteurAsservi.getAngleMoteur(getId()) + Angle::makeFromMilliRad(t.getDonnee<short>(0));
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		// regler coefficient pid
		case 0x07_b:
			if(t.getNbDonnees() == 4) {
				std::cout << "asservissement : changer coefficient pid type=" << (unsigned short)t.getDonnee(0)
				          << " kp=" << (unsigned short)t.getDonnee(1) << " ki=" << (unsigned short)t.getDonnee(2)
				          << " kd=" << (unsigned short)t.getDonnee(3) << std::endl;
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x0A_b:
			std::cout << "desactiver/activer l'asservissement du barillet ou laser !" << std::endl;
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Envoi reponce a un ping
void CarteAsservissement2008::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteAsservissement2008::envoyerPong ping -> pong" << std::endl;
}

// Envoi vitesse atteint
void CarteAsservissement2008::envoyerVitesseAtteint() {
	Trame aEnvoyer(this->getId(), 4);
	aEnvoyer.addByte(0xFF_b); // une donne quelconque
	this->envoyer(aEnvoyer);
	// cout << "CarteAsservissement2008::envoyerVitesseAtteint" << endl;
}

// Envoi position atteint
void CarteAsservissement2008::envoyerPositionAtteint() {
	Trame aEnvoyer(this->getId(), 5);
	aEnvoyer.addByte(0xFF_b); // une donne quelconque
	this->envoyer(aEnvoyer);
	// cout << "CarteAsservissement2008::envoyerPositionAtteint" << endl;
}

// si le mode d'asservissement est position regule la position jusqu'a l'atteindre
void CarteAsservissement2008::update(Duration elapsed) {
	// TODO ameliorer le traiement pour les cas aux limites de M_PI ou -M_PI car pour passer entre ces 2 valeurs on fait
	// un tout complet
	Carte::update(elapsed);
	// si le mode d'asservissement est position
	if(_typeAsservis == ASSERVIS_POSITION) {
		// recuperer l'angle actuel du moteur
		Angle angleActuel = _iMoteurAsservi.getAngleMoteur(getId());
		// si l'angle actuel est presque egal a la commande
		if(-ASSERVIS_EPSILON < angleActuel - _angle && angleActuel - _angle < ASSERVIS_EPSILON) {
			// mode asservissement vitesse (le update ne la modifira plus)
			_typeAsservis = ASSERVIS_VITESSE;
			_vitesse = 0_rad_s;
			// modifier la vitesse du moteur
			_iMoteurAsservi.setVitesseAngulaireMoteur(getId(), _vitesse);
			// prevenir l'ia
			envoyerPositionAtteint();
		} else {
			// calculer une vitesse de commande
			_vitesse = (_angle - angleActuel) * 32 / 1_s;
			// si la vitesse est trop grande ou trop petite la diminuer
			if(_vitesse > ASSERVIS_VITESSE_MAX)
				_vitesse = ASSERVIS_VITESSE_MAX;
			if(_vitesse < -ASSERVIS_VITESSE_MAX)
				_vitesse = -ASSERVIS_VITESSE_MAX;
			// l'envoyer au moteur
			_iMoteurAsservi.setVitesseAngulaireMoteur(getId(), _vitesse);
		}
		// cout << "CarteAsservissement2008::update consigne=" << (short)(angle/M_PI*180.0) << " Actuel=" <<
		// (short)(angleActuel/M_PI*180.0) << " vitesse=" << (short)(vitesse/M_PI*180.0) << endl;
	}
}

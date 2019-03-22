#include "CarteTelemetreIR2008.h"

const unsigned char CarteTelemetreIR2008::NBR_TELEMETRES = 3;

const unsigned char CarteTelemetreIR2008::TELEMETRE_IR_DEVANT_MILIEU = 0;

CarteTelemetreIR2008::CarteTelemetreIR2008(unsigned char id, Robot& robot, ITelemetres& iTelemetres)
        : Carte(id, robot), _iTelemetres(iTelemetres) {}

CarteTelemetreIR2008::~CarteTelemetreIR2008() {}

// Traite les données fournies par l'IA
void CarteTelemetreIR2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				envoyerPong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		case 0x01_b:
			if(t.getNbDonnees() == 1)
				envoyerMesureDistance(t.getDonnee(0));
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;

		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// envoyer une trame de pong
void CarteTelemetreIR2008::envoyerPong() {
	Trame aEnvoyer(this->getId(), 0);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteTelemetreIR2008::pong ping -> pong" << std::endl;
}

// Envoi l'etat des contacteurs...
void CarteTelemetreIR2008::envoyerMesureDistance(uint8_t num) {
	// verifier si le numero du capteur n'et pas trop grand
	if(num >= NBR_TELEMETRES)
		throw ErreurNumTelemetre(num);

	// aller chercher la couleur du capteur dans le robot
	auto distance = _iTelemetres.getTelemetresDistance(getId(), num);

	double tension = 0;
	// conversion mesure distance vers tension electrique mesuree par le capteur
	if(num == TELEMETRE_IR_DEVANT_MILIEU)
		tension = pow(distance.toMm() / 571.3, -1 / 0.9641) * 204.8;
	else
		tension = pow(distance.toMm() / 241.51, -1 / 0.8806) * 204.8;

	// construire la trame
	Trame t(this->getId(), 1);
	t.addDonnees(num, uint16_t(tension));

	// envoyer cette trame
	this->envoyer(t);
}

// Envoie une trame toutes les 200 ms
void CarteTelemetreIR2008::update(Duration elapsed) {
	Carte::update(elapsed);
	static auto tempsDernierEnvoi = 0.0_s;
	static auto tempsActuel = 0.0_s;
	tempsActuel += elapsed;
	if(tempsActuel - tempsDernierEnvoi > 0.2_s) {
		tempsDernierEnvoi = tempsActuel;
		envoyerMesureDistance(0);
		envoyerMesureDistance(1);
		envoyerMesureDistance(2);
	}
}

// CarteBalisesIR2008.cpp

#include "CarteBalisesIR2008.h"

CarteBalisesIR2008::CarteBalisesIR2008(unsigned char id, Robot& robot) : Carte(id, robot) {
	_iBalise = nullptr;
}

CarteBalisesIR2008::~CarteBalisesIR2008() {}

// associer une des balises
void CarteBalisesIR2008::setBalise(const IBalise* balise) {
	_iBalise = balise;
}

// Traite les donnees fournies par l'IA
void CarteBalisesIR2008::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// ping
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// repondre un pong
				pong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		case 0x01_b:
			// demande d'actualisation de l'etat des recepteurs
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				etatRecepteurs();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		case 0x02_b:
			// activation/desactivation envoi auto
			if(t.getNbDonnees() == 1)
				_envoiAuto = (t.getDonnee(0) == 1);
			else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Tous les messages que la carte peut envoyer à l'IA

void CarteBalisesIR2008::pong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteBalises2007::pong ping -> pong" << std::endl;
}

// allumer un recepteur dans la trame corespondante
void CarteBalisesIR2008::setOnRecepteur(unsigned char numRecepteur, Trame& trame) {
	if(numRecepteur < 8)
		trame.set(0, numRecepteur, true);
	else
		trame.set(1, numRecepteur - 8, true);
}

void CarteBalisesIR2008::etatRecepteurs() {
	Trame trame(this->getId(), 0x01_b);
	trame.addByte(0x00_b);
	trame.addByte(0x00_b);
	// si la balise est active
	if(_iBalise) {
		// calculer sont angle avec le robot
		auto deltatX = _iBalise->getPosition().z - this->getRobot().getPosition().z;
		auto deltatY = _iBalise->getPosition().x - this->getRobot().getPosition().x;
		Angle angleBalise = atan2(deltatY, deltatX);
		// soustraire l'angle du robot pour avoir l'angle par rapport au devant du robot
		angleBalise -= this->getRobot().getAngle2d();
		// si dans le robot le recepteur 0 n'est pas bien devant
		// angleBalise += 80.0/180.0*M_PI;
		// ramener l'angle entre 0 et 2PI
		angleBalise = angleBalise.toMod2Pi();
		// le ramener entre 0 et 16 pour avoir le numero du recepteur qui pointe le plus vers la balise
		unsigned char centre = (unsigned char)(angleBalise * 8.0 / 1_PI);
		// mettre a vrai le recepteur corespondant dans une des 2 trames
		setOnRecepteur(centre, trame);
		// mesurer la distance du robot a la balise (en dm)
		Distance distance = sqrt(deltatX * deltatX + deltatY * deltatY);
		// si la distance est inferieure a 1 metre
		if(distance < 1.5_m) {
			// allumer les 2 recepteurs a cote du centre => 3 recepteurs en tout
			setOnRecepteur((centre + 1) % 16, trame);
			setOnRecepteur((centre - 1 + 16) % 16, trame);
		}
		// si la distance est inferieure a 75 cm
		if(distance < 1_m) {
			// allumer 2 recepteurs de plus => 5 recepteurs en tout
			setOnRecepteur((centre + 2) % 16, trame);
			setOnRecepteur((centre - 2 + 16) % 16, trame);
		}
		// si la distance est inferieure a 50 cm
		if(distance < 0.5_m) {
			// allumer 2 recepteurs de plus => 7 recepteurs en tout
			setOnRecepteur((centre + 3) % 16, trame);
			setOnRecepteur((centre - 3 + 16) % 16, trame);
		}
	}
	// cout << "CarteBalises2007::etatRecepteurs trame:" << trame->toString() << endl;
	this->envoyer(trame);
}

// envoie une trame toutes les 200 ms
void CarteBalisesIR2008::update(Duration elapsed) {
	Carte::update(elapsed);
	if(_envoiAuto) {
		static Duration tempsDernierEnvoi = 0.0_s;
		static Duration tempsActuel = 0.0_s;
		tempsActuel += elapsed;
		if(tempsActuel - tempsDernierEnvoi > 0.2_s) {
			tempsDernierEnvoi = tempsActuel;
			etatRecepteurs();
		}
	}
}

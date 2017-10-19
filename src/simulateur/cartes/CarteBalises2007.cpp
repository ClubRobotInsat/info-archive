// CarteContacteurs2007.cpp

#include "CarteBalises2007.h"

CarteBalises2007::CarteBalises2007(unsigned char id, Robot& robot) : Carte(id, robot) {
	for(unsigned char num = 0; num < NBR_BALISES; num++)
		_balises[num] = nullptr;
}

CarteBalises2007::~CarteBalises2007() {}

// associer une des balises
void CarteBalises2007::setBalise(unsigned char numero, const IBalise* balise) {
	if(numero > NBR_BALISES)
		throw ErreurNumBaliseInvalise(numero);
	_balises[numero] = balise;
}

// Traite les donnees fournies par l'IA
void CarteBalises2007::traiterTrame(Trame const& t) {
	switch(t.getCmd()) {
		case 0x00_b:
			// si ping
			if(t.getNbDonnees() == 1 && t.getDonnee(0) == 0x55_b) {
				// repondre un pong
				pong();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		case 0x01_b:
			// si demande d'actualisation de l'etat des recepteurs
			if(t.getNbDonnees() == 1) {
				// envoyer etat des recepteurs
				etatRecepteurs();
			} else
				throw Trame::ErreurTrameNonTraitee(t);
			break;
		default:
			throw Trame::ErreurTrameNonTraitee(t);
	}
}

// Tous les messages que la carte peut envoyer à l'IA

void CarteBalises2007::pong() {
	Trame aEnvoyer(this->getId(), 0x00_b);
	aEnvoyer.addByte(0xAA_b);
	this->envoyer(aEnvoyer);
	std::cout << "CarteBalises2007::pong ping -> pong" << std::endl;
}

// allumer un recepteur dans la trame corespondante
void CarteBalises2007::setOnRecepteur(unsigned char numBalise, unsigned char numRecepteur, Trame& trame1, Trame& trame2) {
	if(numRecepteur < 8)
		trame1.set(numBalise + 1, numRecepteur, true);
	else
		trame2.set(numBalise + 1, numRecepteur - 8, true);
}

void CarteBalises2007::etatRecepteurs() {
	// trame1 reponse a une demande d'actualisation des recepteurs 0 à 7
	Trame trame1(this->getId(), 0x01_b);
	// trame2 reponse a une demande d'actualisation des recepteurs 8 à 15
	Trame trame2(this->getId(), 0x01_b);
	// ajout de la premiere donnee permettant de differencier les 2 trames
	trame1.addByte(0x00_b);
	trame2.addByte(0x01_b);
	for(unsigned char numBalise = 0; numBalise < NBR_BALISES; numBalise++) {
		// ajout d'un octet (etat des recepteurs 0 a 7) pour chaque balise et la trame1
		trame1.addByte(0x00_b);
		// ajout d'un octet (etat des recepteurs 8 a 15) pour chaque balise et la trame2
		trame2.addByte(0x00_b);
		// si la balise est active
		if(_balises[numBalise] != nullptr) {
			// calculer sont angle avec le robot
			Distance deltatX = _balises[numBalise]->getPosition().z - this->getRobot().getPosition().z;
			Distance deltatY = _balises[numBalise]->getPosition().x - this->getRobot().getPosition().x;
			Angle angleBalise = atan2(deltatY, deltatX);
			// soustraire l'angle du robot pour avoir l'angle par rapport au devant du robot
			angleBalise -= this->getRobot().getAngle2d();
			// il semble que dans le robot le recepteur 0 n'est pas bien devant
			angleBalise += 80_deg;
			// ramener l'angle entre 0 et 2PI
			angleBalise = angleBalise.toMod2Pi();
			// le ramener entre 0 et 16 pour avoir le numero du recepteur qui pointe le plus vers la balise
			unsigned char centre = (unsigned char)(angleBalise * 8.0 / 1_PI);
			// mettre a vrai le recepteur corespondant dans une des 2 trames
			setOnRecepteur(numBalise, centre, trame1, trame2);
			// mesurer la distance du robot a la balise (en dm)
			auto distance = sqrt(deltatX * deltatX + deltatY * deltatY);
			// si la distance est inferieure a 1 metre
			if(distance < 1.5_m) {
				// allumer les 2 recepteurs a cote du centre => 3 recepteurs en tout
				setOnRecepteur(numBalise, (centre + 1) % 16, trame1, trame2);
				setOnRecepteur(numBalise, (centre - 1 + 16) % 16, trame1, trame2);
			}
			// si la distance est inferieure a 75 cm
			if(distance < 1_m) {
				// allumer 2 recepteurs de plus => 5 recepteurs en tout
				setOnRecepteur(numBalise, (centre + 2) % 16, trame1, trame2);
				setOnRecepteur(numBalise, (centre - 2 + 16) % 16, trame1, trame2);
			}
			// si la distance est inferieure a 50 cm
			if(distance < 0.5_m) {
				// allumer 2 recepteurs de plus => 7 recepteurs en tout
				setOnRecepteur(numBalise, (centre + 3) % 16, trame1, trame2);
				setOnRecepteur(numBalise, (centre - 3 + 16) % 16, trame1, trame2);
			}
		}
	}
	// cout << "CarteBalises2007::etatRecepteurs trame1:" << trame1->toString() << endl;
	this->envoyer(trame1);
	// cout << "CarteBalises2007::etatRecepteurs trame2:" << trame2->toString() << endl;
	this->envoyer(trame2);
}

// envoie une trame toutes les 200 ms
void CarteBalises2007::update(Duration elapsed) {
	Carte::update(elapsed);
	static Duration tempsDernierEnvoie = 0.0_s;
	static Duration tempsActuel = 0.0_s;
	tempsActuel += elapsed;
	if(tempsActuel - tempsDernierEnvoie > 0.2_s) {
		tempsDernierEnvoie = tempsActuel;
		etatRecepteurs();
	}
}

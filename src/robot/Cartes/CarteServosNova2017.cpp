/************************************************************
 * CarteServos2017.cpp *
 * Implémente la classe gérant les servos 2017 *
 ************************************************************/

#include "CarteServosNova2017.h"
#include <cstring>

CarteServosNova2017::CarteServosNova2017(Commun::CAN& can, uint8_t id) : CarteServo(can, id) {
	std::memset(_positions, 0, NB_MAX_SERVOS_NOVA);
	std::memset(_termine, true, NB_MAX_SERVOS_NOVA);
	std::memset(_bloque, false, NB_MAX_SERVOS_NOVA);
}

void CarteServosNova2017::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case RETOUR_POSITION:
			if(message.getNbDonnees() == 3 && message.getDonnee(0) < NB_MAX_SERVOS_NOVA) {
				bloquerVariables();
				_positionActualisee[message.getDonnee(0)] = true;
				{
					std::lock_guard<std::mutex> lk(_mutexPositions);
					_positions[message.getDonnee(0)] = Angle::makeFromDeg(message.getDonnee<uint16_t>(1));
				}
				debloquerVariables();
			}
			break;

		case TERMINE:
			if(message.getNbDonnees() == 2 && message.getDonnee(0) < NB_MAX_SERVOS_NOVA) {
				_termine[message.getDonnee(0)] = message.getDonnee(1);
			}
			break;

		case BLOQUE:
			if(message.getNbDonnees() == 2 && message.getDonnee(0) < NB_MAX_SERVOS_NOVA) {
				_bloque[message.getDonnee(0)] = message.getDonnee(1);
				// printf("Servo Bloque : ID = %d, Num = %d\n", getId(), donnees[0]);
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Règle la position d'un servo
void CarteServosNova2017::reglerModeBlocage(uint8_t servo, ModeBlocage mode) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return;
	}

	envoyerMessage(this->make_trame(MODE_BLOCAGE, servo, mode));
}

// Règle la position d'un servo
void CarteServosNova2017::reglerPosition(uint8_t servo, Angle angle) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return;
	}

	bloquerVariables();
	_termine[servo] = false;
	_bloque[servo] = false;
	debloquerVariables();


	std::lock_guard<std::mutex> lk(_mutexPositions);
	envoyerMessage(this->make_trame(REGLER_POSITION, servo, (angle).toDeg<uint16_t>()));
}

void CarteServosNova2017::actualiserPosition(uint8_t servo) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return;
	}

	_positionActualisee[servo] = false;

	envoyerMessage(RETOUR_POSITION, servo);
}

bool CarteServosNova2017::verifierPositionActualisee(uint8_t servo) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return true;
	}

	return _positionActualisee[servo].exchange(false);
}

// Retourne la position actuelle du servo voulu
Angle CarteServosNova2017::lirePosition(uint8_t servo) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demandé : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		throw std::runtime_error("Servo invalide!");
	}

	std::lock_guard<std::mutex> lk(_mutexPositions);
	return _positions[servo];
}

// Verifie si le dernier ordre envoye au servo est execute ou pas encore
bool CarteServosNova2017::verifierDeplacementTermine(uint8_t servo) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return true;
	}

	return _termine[servo];
}

// Verifie si le servo en question est _bloque ou pas
bool CarteServosNova2017::verifierServoBloque(uint8_t servo) {
	if(servo >= NB_MAX_SERVOS_NOVA) {
		logError("numero du servo demande : ", (int)servo, " >= ", NB_MAX_SERVOS_NOVA);
		return true;
	}

	return _bloque[servo];
}

void CarteServosNova2017::setServoVitesse(uint8_t servo, uint8_t vitesse) {
	envoyerMessage(this->make_trame(VITESSE, servo, vitesse));
}

void CarteServosNova2017::setCouleur(uint8_t servo, Couleur couleur) {
	envoyerMessage(this->make_trame(COULEUR, servo, couleur));
}
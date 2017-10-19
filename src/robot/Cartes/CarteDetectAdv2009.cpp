/*************************************************************
 * CarteDetectAdv2009.cpp *
 * Spécifie la classe gérant la balise rotative pour la detection de l'adversaire 2008/2009 *
 *************************************************************/

#include "CarteDetectAdv2009.h"
//#include "CarteDetectAdv2009_Erreurs.h"

CarteDetectAdv2009::CarteDetectAdv2009(Commun::CAN& can, uint8_t id) : CarteAsservissement2009(can, id) {
	_diametreAngulaire = 0_rad;
	_angle = 0_rad;
	_distance = 0_mm;
}

void CarteDetectAdv2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		// Gestion des messages specifiques a la carte detect adv, le reste est gere par asserv 2009
		case 0xE: // Mesure de diametre angulaire et _angle
			if(message.getNbDonnees() == 8) {
				std::lock_guard<std::mutex> lk(_mutexVariables);
				_mesureAdvActualise = true;
				_diametreAngulaire = Angle::makeFromRad(message.getDonnee<float>(0));
				_angle = Angle::makeFromRad(message.getDonnee<float>(4));
				// logError("Error :", Angle::makeFromRad(message.getDonnee<float>(4)));
				_distance = Distance::makeFromMm(message.getDonnee<float>(0));
				// logDebug("Réception message carte adversaire _distance=", _distance, " _angle=", _angle);
			}
			break;

		default:
			CarteAsservissement2009::traiterMessage(message);
			break;
	}
}

void CarteDetectAdv2009::actualiserMesureAdv() {
	_mesureAdvActualise = false;

	envoyerMessage(DETECT, 0x02_b);
}

bool CarteDetectAdv2009::verifierMesureAdvActualise() {
	return _mesureAdvActualise.exchange(false);
}

Angle CarteDetectAdv2009::getDiametreAngulaire() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _diametreAngulaire;
}

Angle CarteDetectAdv2009::getAngle() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _angle;
}

// pour le simu
Distance CarteDetectAdv2009::getDistance() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return _distance;
}

void CarteDetectAdv2009::activerEnvoiMesureAdvAuto() {
	_mesureAdvActualise = false;

	envoyerMessage(DETECT, 0x01_b);
}

void CarteDetectAdv2009::desactiverEnvoiMesureAdvAuto() {
	envoyerMessage(DETECT, 0x00_b);
}

/*************************************************************
 * CarteAsservissement2009.cpp *
 * Implémente la classe gérant le déplacement 2008/2009 *
 *************************************************************/

#include "CarteAsservissement2009.h"

CarteAsservissement2009::CarteAsservissement2009(Commun::CAN& can, uint8_t id)
        : CarteAsservissement(can, id)
        , _angleOk{false}
        , _bloque{false}
        , _deplacement{DEPLACEMENT_RIEN}
        , _deplacementTermine{true}
        , _precisionAtteinte{true}
        , _mesureOk{false}
        , _ack{ACK_RIEN} {}

void CarteAsservissement2009::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case ACK: // Message ignoré
			if(message.getNbDonnees() == 1) {
				_ack = (TypeAck)message.getDonnee(0);
			}
			break;

		case BLOQUE: // Notification de Blocage
			_bloque = true;
			// printf("CarteAsservissement2009 : Bloquage Recu !\n");
			break;

		case ANGLE: // Angle
			if(message.getNbDonnees() == 4) {
				std::lock_guard<std::mutex> lk(_mutexAngle);
				_angle = Angle::makeFromMilliRad(message.getDonnee<int32_t>(0));
				_angleOk = true;
			}
			break;

		case TERMINE: // Déplacement terminé
			if(message.getNbDonnees() == 1) {
				if(message.getDonnee(0) == 1) { // Deplacement termine <=> Précision atteinte + Vitesse longitudinale du robot = 0.0 {
					_precisionAtteinte = true;
					_deplacementTermine = true;
				} else if(message.getDonnee(0) == 2) { // Précision atteinte : on est rentré dans un carré de
					// Mouvement_PrecisionLongitudinale*Mouvement_PrecisionLongitudinale
					// (ou angulaire) autour de la cible {
					_precisionAtteinte = true;
				} else {
					_precisionAtteinte = false;
					_deplacementTermine = false;
				}
			}
			break;

		case MESURE: // On recoit une _mesure de vitesse position etc
			if(message.getNbDonnees() == 8) {
				_mesure[0] = message.getDonnee<std::int16_t>(1);
				_mesure[1] = message.getDonnee<std::int16_t>(3);
				_mesure[2] = message.getDonnee<std::int16_t>(5);
				_mesure[3] = message.getDonnee(7);
				_mesureOk = true;
			}
			break;

		case GET_VARIABLE: // On recoit la valeur d'une variable
			if(message.getNbDonnees() == 4) {
				_parametre = message.getDonnee<float>(0);
				_parametreOk = true;
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Fait redemarrer la carte
void CarteAsservissement2009::reset() {
	envoyerMessage(RESET, 0xFF_b);
}

// Actualise l'angle actuel du moteur
void CarteAsservissement2009::actualiserAngle() {
	_angleOk = false;
	_ack = ACK_RIEN;

	envoyerMessage(ANGLE, 0xFF_b);
}

// Verifie que l'angle actualise est arrive
bool CarteAsservissement2009::angleActualise() {
	return _angleOk;
}

// Retourne l'angle du moteur
Angle CarteAsservissement2009::lireAngle() {
	std::lock_guard<std::mutex> lk(_mutexAngle);
	return _angle;
}

// Definit l'angle actuel du moteur
void CarteAsservissement2009::definirAngle(Angle angle) {
	_ack = ACK_RIEN;

	Trame t = this->make_trame(DEFINIR_ANGLE, angle.toMilliRad<int>());
	envoyerMessage(t);
}

// Retourne le déplacement en cours
bool CarteAsservissement2009::verifierDeplacementTermine() {
	return _deplacementTermine;
}

bool CarteAsservissement2009::verifierPrecisionAtteinte() {
	return _precisionAtteinte;
}

// Retourne si le moteur est _bloque ou pas
bool CarteAsservissement2009::verifierBlocage() {
	return _bloque.exchange(false);
}

// S'arréter a l'angle actuel
void CarteAsservissement2009::arreter() {
	_ack = ACK_RIEN;
	_deplacementTermine = true;
	_deplacement = DEPLACEMENT_ARRETER;
	_precisionAtteinte = true;
	_bloque = false;

	envoyerMessage(COMMANDE, _deplacement);
}

// Tourner de l'angle donné
void CarteAsservissement2009::tourner(Angle angle) {
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_precisionAtteinte = false;
	_deplacement = DEPLACEMENT_TOURNER;
	_bloque = false;

	Trame t = this->make_trame(COMMANDE, _deplacement, angle.toMilliRad<int>());
	envoyerMessage(t);
}

// Tourner jusqu'à avoir l'angle absolu donné
void CarteAsservissement2009::tournerAbsolu(Angle angle) {
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_precisionAtteinte = false;
	_deplacement = DEPLACEMENT_TOURNER_ABSOLU;
	_bloque = false;

	Trame t = this->make_trame(COMMANDE, _deplacement, angle.toMilliRad<int>(), SensRotation::Trigo);
	envoyerMessage(t);
}

// Tourner à vitesse constante jusqu'à nouvel ordre
void CarteAsservissement2009::tournerInfini(AngularSpeed vitesse, SensRotation sens) {
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_precisionAtteinte = false;
	_deplacement = DEPLACEMENT_TOURNER_INFINI;
	_bloque = false;

	Trame t = this->make_trame(COMMANDE, _deplacement, vitesse.toMilliRad_s<int32_t>(), sens);
	envoyerMessage(t);
}

// Arret d'urgence
void CarteAsservissement2009::arretUrgence() {
	_ack = ACK_RIEN;
	_deplacementTermine = true;
	_precisionAtteinte = true;
	_deplacement = DEPLACEMENT_ARRET_URGENCE;
	_bloque = false;

	envoyerMessage(COMMANDE, _deplacement);
}

// Allume ou éteint les LED temoins
void CarteAsservissement2009::allumerLed(bool led1, bool led2) {
	_ack = ACK_RIEN;

	Trame t = this->make_trame(LED, uint8_t(led1), uint8_t(led2));
	envoyerMessage(t);
}

// Actualise les données de debug demandées
void CarteAsservissement2009::demanderMesure() {
	_mesureOk = false;
	_ack = ACK_RIEN;

	envoyerMessage(MESURE, 0xFF_b);
}

// Vérifie que les données de debug soient actualisées
bool CarteAsservissement2009::mesureActualisee() {
	return _mesureOk;
}

// Lit une donnée de debug
short CarteAsservissement2009::lireMesure(TypeMesure type) {
	return _mesure[(int)type];
}

// Permet d'activer ou de désactiver l'asservissement en angle
void CarteAsservissement2009::activerAsservissementAngle(bool active) {
	_ack = ACK_RIEN;

	envoyerMessage(ASSERV_ON_OFF, active);
}


// Vérifie qu'un _ack ait été reçu
CarteAsservissement2009::TypeAck CarteAsservissement2009::ackRecu() {
	CarteAsservissement2009::TypeAck resultat = ACK_RIEN;

	if(_ack != ACK_RIEN) {
		resultat = _ack;
		_ack = ACK_RIEN;
	}

	return resultat;
}


void CarteAsservissement2009::activerEnvoiAuto(bool on_off) {
	_ack = ACK_RIEN;

	envoyerMessage(ENVOI_AUTO, on_off);
}

// Définit la variable 'param' (voir les define dans le .h) à la valeur 'val'
void CarteAsservissement2009::reglerParametre(uint8_t param, float val) {
	_ack = ACK_RIEN;

	Trame t = this->make_trame(SET_VARIABLE, param, val);
	envoyerMessage(t);
}

// Demande la valeur de la variable 'param' (voir les define dans le .h)
void CarteAsservissement2009::demanderParametre(uint8_t param) {
	_parametreOk = false;
	_ack = ACK_RIEN;

	envoyerMessage(GET_VARIABLE, param);
}

bool CarteAsservissement2009::parametreActualise() {
	return _parametreOk;
}

// Lit une donnée de debug
float CarteAsservissement2009::lireParametre() {
	return _parametre;
}

/************************************************************
 * CarteDeplacement2007.cpp *
 * Implémente la classe gérant le déplacement 2006/2007 *
 ************************************************************/

#include "CarteDeplacement2007.h"

CarteDeplacement2007::CarteDeplacement2007(CAN& can, uint8_t id)
        : Carte(can, id)
        , _coordsOk(false)
        , _bloque(false)
        , _deplacement(DEPLACEMENT_RIEN)
        , _deplacementTermine(false)
        , _debugOk(false)
        , _ack(ACK_RIEN) {}

void CarteDeplacement2007::traiterMessage(Trame const& message) {

	switch(message.getCmd()) {

		case 0x01_b: // Message ignoré
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_ack = ACK_MESSAGE_IGNORE;
				debloquerVariables();
			}
			break;
		case 0x02_b: // Message executé
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_ack = ACK_MESSAGE_EXECUTE;
				debloquerVariables();
			}
			break;

		case 0x04_b: // Notification de blocage
			bloquerVariables();
			_bloque = true;
			debloquerVariables();
			printf("CarteDeplacement2007 : Blocage recu !\n");
			break;

		case 0x05_b: // Coordonnées
			if(message.getNbDonnees() == 6) {
				bloquerVariables();
				_coords.x = message.getDonnee<std::int16_t>(0);
				_coords.y = message.getDonnee<std::int16_t>(2);
				_coords.theta = message.getDonnee<std::int16_t>(4);
				_coordsOk = true;
				debloquerVariables();
			}
			break;
		case 0x06_b: // Déplacement terminé
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				if(message.getDonnee(0) == 1)
					_deplacementTermine = true;
				else
					_deplacementTermine = false;
				debloquerVariables();
			}
			break;
		case 0x09_b: // Configuration actuelle
			if(message.getNbDonnees() == 7) {
				bloquerVariables();
				_debug[0] = message.getDonnee<std::int16_t>(1);
				_debug[1] = message.getDonnee<std::int16_t>(3);
				_debug[2] = message.getDonnee<std::int16_t>(5);
				_debugOk = true;
				debloquerVariables();
			}
			break;
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Actualise les coordonnées du robot
void CarteDeplacement2007::actualiserCoordonnees() {
	_coordsOk = false;

	envoyerMessage(0x05_b, 0xFF_b);
}

// Vérifie que les coordonnées du robot sont actualisées
bool CarteDeplacement2007::coordonneesActualisees() {
	return _coordsOk;
}

// Retourne les coordonnées du robot
CoordonneesOld CarteDeplacement2007::lireCoordonnees() {
	CoordonneesOld resultat;

	bloquerVariables();
	resultat = _coords;
	debloquerVariables();

	return resultat;
}

// Definit les coordonnées actuelles du robot
void CarteDeplacement2007::definirCoordonnees(std::int16_t x, std::int16_t y, std::int16_t a) {
	envoyerMessage(this->make_trame(0x0A_b, x, y, a));
}

// Retourne le déplacement en cours
bool CarteDeplacement2007::verifierDeplacementTermine() {
	return _deplacementTermine;
}

// Indique si le robot est bloqué ou pas
bool CarteDeplacement2007::verifierBlocage() {
	return _bloque.exchange(false);
}

// S'arréter aux coordonnées actuelles
void CarteDeplacement2007::arreter() {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(0x07_b, 0x00_b);
}

// Avancer de la distance donnée (mm)
void CarteDeplacement2007::avancer(std::int16_t distance) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(1), distance));
}

// Tourner de l'angle donné (mrad)
void CarteDeplacement2007::tourner(std::int16_t angle) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_TOURNER;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(2), angle));
}

// Tourner jusqu'à avoir l'angle absolu donné (mrad)
void CarteDeplacement2007::tournerAbsolu(std::int16_t theta, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_TOURNER_ABSOLU;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(3), theta, sens));
}

// Se tourner vers le point donné (mm, mm)
void CarteDeplacement2007::pointerVers(std::int16_t x, std::int16_t y, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_POINTER_VERS;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(4), x, y, sens));
}

// Se rend au point donné (mm, mm) par une composition de rotation et d'avance, imprécis => déprécié
void CarteDeplacement2007::vaVers(std::int16_t x, std::int16_t y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_ALLER_VERS;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(5), x, y, sens));
}

// Se rendre au point donné (mm, mm) par une composition de rotation et d'avance
void CarteDeplacement2007::allerADecompose(std::int16_t x, std::int16_t y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_ALLER_A_DECOMPOSE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(6), x, y, sens));
}

// Se rendre au point donné (mm, mm) par une trajectoire lissée
void CarteDeplacement2007::allerA(std::int16_t x, std::int16_t y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_ALLER_A;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(7), x, y, sens));
}

// Avancer à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2007::avancerInfini(SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_AVANCER_INFINI;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(8), sens));
}

// Tourner à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2007::tournerInfini(SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_TOURNER_INFINI;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(9), sens));
}


// Se rendre au point donné (mm, mm) par une trajectoire lissée sans s'y arréter
void CarteDeplacement2007::passerPar(std::int16_t x, std::int16_t y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_PASSER_PAR;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(10), x, y, sens));
}

// Pivote de l'angle donné (mrad) autour de la roue droite
void CarteDeplacement2007::pivoterDroite(std::int16_t angle, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_PIVOTER_DROITE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(11), angle, sens));
}

// Pivote de l'angle donné (mrad) autour de la roue gauche
void CarteDeplacement2007::pivoterGauche(std::int16_t angle, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_PIVOTER_GAUCHE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(12), angle, sens));
}

// Fait avancer chacune des 2 roues d'une distance donnée (mm, mm)
void CarteDeplacement2007::commanderRoues(std::int16_t droite, std::int16_t gauche) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_COMMANDER_ROUES;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(13), droite, gauche));
}

// changer la vitesse individuelle des roues quand on les commande individuellement
void CarteDeplacement2007::commanderVitesseRoues(SensAvance sens) {
	envoyerMessage(this->make_trame(0x07_b, uint8_t(14), sens));
}

// Fait avancer chacune des 2 roues d'une distance donnée (mm, mm)
void CarteDeplacement2007::arretUrgence() {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_ARRET_URGENCE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(0x07_b, 0x15_b);
}

// Permet de contrôler en direct le déplacement angulaire et longitudinal du robot
void CarteDeplacement2007::controleTrajectoire(std::int16_t distance, std::int16_t angle, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_CONTROLE_TRAJECTOIRE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(16), distance, angle, sens));
}

// Permet de contrôler en direct la vitesse longitudinale et angulaire du robot
void CarteDeplacement2007::controleVitesseLongAng(std::int16_t vitesse_longitudinale, std::int16_t vitesse_angulaire) {
	bloquerVariables();
	_ack = ACK_RIEN;
	_deplacementTermine = false;
	_deplacement = DEPLACEMENT_CONTROLE_TRAJECTOIRE;
	_bloque = false;
	debloquerVariables();

	envoyerMessage(this->make_trame(0x07_b, uint8_t(17), vitesse_longitudinale, vitesse_angulaire));
}

// Allume ou éteint les LED temoins
void CarteDeplacement2007::reglerTemoin(bool led1, bool led2) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x08_b, led1, led2));
}

// Actualise les données de _debug demandées
void CarteDeplacement2007::actualiserMesure(TypeMesure parametres) {
	_debugOk = false;

	envoyerMessage(0x09_b, parametres);
}

// Vérifie que les données de _debug soient actualisées
bool CarteDeplacement2007::mesureActualisee() {
	return _debugOk;
}

// Lit une donnée de _debug
std::int16_t CarteDeplacement2007::lireMesure(uint8_t donnee) {
	if(donnee >= 3)
		return 0;

	return _debug[donnee];
}

// Règle les coefficients des correcteurs PID
void CarteDeplacement2007::reglerPID(TypeGrandeur type, std::int16_t kp, std::int16_t ki, std::int16_t kd) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x03_b, PID, type, kp, ki, kd));
}

// Permet d'activer ou de désactiver certains types d'asservissement (distance, angle, roue droite, roue gauche)
void CarteDeplacement2007::activerAsservissement(TypeGrandeur type, bool active) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x08_b, type, active));
}


// Règle la vitesse_max ou l'acceleration_max du robot
void CarteDeplacement2007::reglerCinetique(TypeGrandeur type, TypeCinetique cinetique, std::int16_t valeur) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x03_b, CINETIQUE, type, cinetique, valeur));
}


// Règle la précision des mouvements
void CarteDeplacement2007::reglerPrecision(TypeGrandeur type, std::int16_t precision) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x03_b, PRECISION, type, precision));
}


// Règle quelques grandeurs mécaniques (entre_axe, diamètre des roues)
void CarteDeplacement2007::reglerMecanique(TypeMeca type, double valeur) {
	std::int16_t val = valeur * 100;
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(0x03_b, MECANIQUE, type, val));
}

// Vérifie qu'un _ack ait été reçu
bool CarteDeplacement2007::ackRecu() {
	bool resultat;

	bloquerVariables();
	resultat = (_ack != ACK_RIEN);
	_ack = ACK_RIEN;
	debloquerVariables();

	return resultat;
}


void CarteDeplacement2007::activerEnvoiAuto() {
	envoyerMessage(0x0C_b, 0x01_b);
}

void CarteDeplacement2007::desactiverEnvoiAuto() {
	envoyerMessage(0x0C_b, 0x00_b);
}

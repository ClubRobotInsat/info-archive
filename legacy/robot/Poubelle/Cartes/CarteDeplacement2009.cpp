/*************************************************************
 * CarteDeplacement2009.cpp *
 * Implémente la classe gérant le déplacement 2008/2009 *
 *************************************************************/

#include "CarteDeplacement2009.h"

const Duration PERIODE_ENVOI_AUTO = 200_ms;

CarteDeplacement2009::CarteDeplacement2009(Commun::CAN& can, uint8_t id)
        : CarteDeplacement(can, id)
        , _coords()
        , _coordsOk(false)
        , _bloque(false)
        , _deplacement(DEPLACEMENT_RIEN)
        , _deplacementTermine(false)
        , _precisionAtteinte(false)
        , _mesureOk(false)
        , _ack(ACK_RIEN) {
	this->setPrecisionAtteinte(true);
	this->setDeplacementTermine(true);
	this->setBloque(false);
}

void CarteDeplacement2009::traiterMessage(Trame const& message) {
	Coordonnees oldCoord;
	switch(message.getCmd()) {

		case ACK: // Message ignoré
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				_ack = (TypeAck)message.getDonnee(0);
				if(_ack == TypeAck::ACK_MESSAGE_IGNORE) {
					logError("Erreur, dernier message envoyé invalide !");
				}
				debloquerVariables();
			}
			break;

		case BLOQUE: // Notification de Blocage
			this->setBloque(true);
			printf("CarteDeplacement2009 : Blocage reçu !\n");
			break;

		case COORD: // Coordonnées
			if(message.getNbDonnees() == 6) {
				bloquerVariables();
				// TODO Vérifier qu'est-ce qu'on reçoit de chez les élecs
				oldCoord = _coords;
				_coords.setPos3D({Distance::makeFromMm(message.getDonnee<std::int16_t>(0)),
				                  Distance::makeFromMm(message.getDonnee<std::int16_t>(2)),
				                  0_m});
				_coords.setAngle(Angle::makeFromMilliRad(message.getDonnee<std::int16_t>(4)));

				// logDebug("Coordonnées reçues", _coords.getPosition(), " ", _coords.theta);
				// On considère que le robot ne peut pas passer d'un coup sur théta = 0 si théta était lointain
				// On considère aussi que le robot ne peut pas aller à x=0 ou y=0
				if((_coords.getX() == 0_mm && oldCoord.getX() != 0_mm) || (_coords.getY() == 0_mm && oldCoord.getY() != 0_mm) ||
				   (abs(_coords.getAngle() - oldCoord.getAngle()) >= 0.5_rad && _coords.getAngle() == 0_rad)) {
					_coords = oldCoord;
				}

				debloquerVariables();
			}
			break;

		case TERMINE: // Déplacement terminé
			if(message.getNbDonnees() == 1) {
				bloquerVariables();
				if(message.getDonnee(0) == 1) { // Deplacement termine <=> Précision atteinte + Vitesse longitudinale du robot = 0.0 {
					this->setDeplacementTermine(true);
				} else if(message.getDonnee(0) == 2) { // Précision atteinte : on est rentré dans un carré de
					// Mouvement_PrecisionLongitudinale*Mouvement_PrecisionLongitudinale
					// (ou angulaire) autour de la cible {
					this->setPrecisionAtteinte(true);
				} else {
					this->setPrecisionAtteinte(false);
					this->setDeplacementTermine(false);
				}
				debloquerVariables();
			}
			break;

		case MESURE: // On recoit une mesure de vitesse position etc
			if(message.getNbDonnees() == 8) {
				bloquerVariables();
				_mesure[0] = message.getDonnee<std::int16_t>(1);
				_mesure[1] = message.getDonnee<std::int16_t>(3);
				_mesure[2] = message.getDonnee<std::int16_t>(5);
				_mesure[3] = message.getDonnee(7);
				_mesureOk = true;
				debloquerVariables();
			}
			break;

		case GET_VARIABLE: // On recoit la valeur d'une variable
			if(message.getNbDonnees() == 4) {
				bloquerVariables();
				_parametre = message.getDonnee<float>(0);
				_parametreOk = true;
				debloquerVariables();
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

void CarteDeplacement2009::setBloque(bool bloque) {
	_bloque = bloque;
}

void CarteDeplacement2009::setDeplacementTermine(bool termine) {
	_deplacementTermine = termine;
}

void CarteDeplacement2009::setPrecisionAtteinte(bool atteinte) {
	_precisionAtteinte = atteinte;
}

// Fait redemarrer la carte
void CarteDeplacement2009::reset() {
	envoyerMessage(RESET, 0xFF);
}

// Actualise les coordonnées du robot
/*void CarteDeplacement2009::actualiserCoordonnees() {
 unsigned char donnees[] = { 0xFF };

 bloquerVariables();
 _ack = ACK_RIEN;
 debloquerVariables();

 envoyerMessage(COORD, sizeof(donnees), donnees);
 }

 // Vérifie que les coordonnées du robot soient actualisées
 bool CarteDeplacement2009::coordonneesActualisees() {
 bool resultat;

 bloquerVariables();
 resultat = _coordsOk;
 debloquerVariables();

 return resultat;
 }*/

// Retourne les coordonnées du robot
Coordonnees CarteDeplacement2009::lireCoordonnees() {
	Coordonnees resultat;

	bloquerVariables();
	resultat = _coords;
	debloquerVariables();

	return resultat;
}

// Definit les coordonnees actuels du robot
void CarteDeplacement2009::definirCoordonnees(const Coordonnees& pos) {
	_ack = ACK_RIEN;

	Trame t = this->make_trame(DEFINIR_COORD,
	                           (pos.getX().toMm<std::int16_t>()),
	                           (pos.getY().toMm<std::int16_t>()),
	                           pos.getAngle().toMilliRad<std::int16_t>());
	envoyerMessage(t);
}

// Retourne le déplacement en cours
bool CarteDeplacement2009::verifierDeplacementTermine() {
	return _deplacementTermine.exchange(false);
}

bool CarteDeplacement2009::verifierPrecisionAtteinte() {
	return _precisionAtteinte.exchange(false);
}

// Retourne si le robot est bloqué ou pas
bool CarteDeplacement2009::verifierNouveauBlocage() {
	return _bloque.exchange(false);
}

// S'arréter aux coordonnées actuelles
void CarteDeplacement2009::arreter() {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_ARRETER;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(COMMANDE, _deplacement);
}

// Avancer de la distance donnée
void CarteDeplacement2009::avancer(Distance distance) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_AVANCER;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, distance.toMm<std::int16_t>()));
	logDebug6("==== CARTE DEPLACEMENT AVANCER ===");
}

// Tourner de l'angle donné
void CarteDeplacement2009::tourner(Angle angle) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_TOURNER;
	this->setBloque(false);
	debloquerVariables();
	auto trame = this->make_trame(COMMANDE, _deplacement, angle.toMinusPiPi().toMilliRad<std::int16_t>());
	envoyerMessage(trame);
}

// Tourner jusqu'à avoir l'angle absolu donné
void CarteDeplacement2009::tournerAbsolu(Angle theta, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_TOURNER_ABSOLU;
	this->setBloque(false);
	debloquerVariables();
	// logDebug3("Angle absolu envoyé à la carte : ", theta.angleMod2Pi().toMilliRad<std::int16_t>());
	auto trame = this->make_trame(COMMANDE, _deplacement, theta.toMod2Pi().toMilliRad<std::int16_t>(), sens);
	envoyerMessage(trame);
}

// Se tourner vers le point donné
void CarteDeplacement2009::pointerVers(const Coordonnees& coords, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_POINTER_VERS;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE,
	                                _deplacement,
	                                (coords.getX(getReference()).toMm<std::int16_t>()),
	                                (coords.getY(getReference()).toMm<std::int16_t>()),
	                                sens));
}

// Se rend au point donné par une composition de rotation et d'avance, imprécis => déprécié
void CarteDeplacement2009::vaVers(Distance x, Distance y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_ALLER_VERS;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, (x.toMm<std::int16_t>()), (y.toMm<std::int16_t>()), sens));
}

// Se rendre au point donné par une composition de rotation et d'avance
void CarteDeplacement2009::allerADecompose(Distance x, Distance y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_ALLER_A_DECOMPOSE;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, (x.toMm<std::int16_t>()), (y.toMm<std::int16_t>()), sens));
}

// Se rendre au point donné par une trajectoire lissée
void CarteDeplacement2009::allerA(Distance x, Distance y, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_ALLER_A;
	this->setBloque(false);
	debloquerVariables();
	auto trame = this->make_trame(COMMANDE, _deplacement, (x.toMm<std::int16_t>()), (y.toMm<std::int16_t>()), sens);
	logDebug3("Trame allerA : ", trame);
	envoyerMessage(trame);
}

// Avancer à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2009::avancerInfini(Speed vitesse, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_AVANCER_INFINI;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, vitesse.toMm_s<std::int16_t>(), sens));
}

// Tourner à vitesse constante jusqu'à nouvel ordre
void CarteDeplacement2009::tournerInfini(AngularSpeed vitesse, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_TOURNER_INFINI;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, vitesse.toMilliRad_s<std::int16_t>(), sens));
}


// Se rendre au point donné par une trajectoire lissée sans s'y arrêter
void CarteDeplacement2009::passerPar(Coordonnees coords, Speed vitesse, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_PASSER_PAR;
	this->setBloque(false);
	debloquerVariables();

	// On passe en paramètre 'false' qui signifie qu'on ne veut pas renvoyer le message
	// si il se perd en route (car sinon le robot danse)
	envoyerMessage(this->make_trame(COMMANDE,
	                                _deplacement,
	                                (coords.getX(REFERENCE).toMm<std::int16_t>()),
	                                (coords.getY(REFERENCE).toMm<std::int16_t>()),
	                                vitesse.toMm_s<std::int16_t>(),
	                                sens),
	               false);
}

// Pivote de l'angle donné autour de la roue droite
void CarteDeplacement2009::pivoterDroite(Angle angle, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_PIVOTER_DROITE;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, angle.toMilliRad<std::int16_t>(), sens));
}

// Pivote de l'angle donné autour de la roue gauche
void CarteDeplacement2009::pivoterGauche(Angle angle, SensRotation sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_PIVOTER_GAUCHE;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, angle.toMilliRad<std::int16_t>(), sens));
}

// Fait avancer chacune des 2 roues d'une distance donnée
void CarteDeplacement2009::positionDroiteGauche(Distance droite, Distance gauche, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_DISTANCE_DG;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, (droite.toMm<std::int16_t>()), (gauche.toMm<std::int16_t>()), sens));
}

// changer la vitesse individuelle des roue quand on les commandes individuellement
void CarteDeplacement2009::vitesseDroiteGauche(Speed vitesse_d, Speed vitesse_g, SensAvance sens) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_VITESSE_DG;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE, _deplacement, vitesse_d.toMm_s<std::int16_t>(), vitesse_g.toMm_s<std::int16_t>(), sens));
}

// Fait avancer chacune des 2 roues d'une distance donnée
void CarteDeplacement2009::arretUrgence() {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_ARRET_URGENCE;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(COMMANDE, _deplacement);
}

// Permet de contrôler en direct la vitesse longitudinale et angulaire du robot
void CarteDeplacement2009::controleVitesseLongAng(Speed vitesse_longitudinale, AngularSpeed vitesse_angulaire) {
	bloquerVariables();
	_ack = ACK_RIEN;
	this->setPrecisionAtteinte(false);
	this->setDeplacementTermine(false);
	_deplacement = DEPLACEMENT_CONTROLE_VITESSE;
	this->setBloque(false);
	debloquerVariables();

	envoyerMessage(this->make_trame(COMMANDE,
	                                _deplacement,
	                                vitesse_longitudinale.toMm_s<std::int16_t>(),
	                                vitesse_angulaire.toMilliRad_s<std::int16_t>()));
}

// Allume ou éteint les LED temoins
void CarteDeplacement2009::allumerLed(bool led1, bool led2) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(COMMANDE, led1, led2));
}

// Actualise les données de debug demandées
void CarteDeplacement2009::demanderMesure(TypeGrandeur grandeur) {
	_mesureOk = false;
	_ack = ACK_RIEN;

	envoyerMessage(MESURE, grandeur);
}

// Vérifie que les données de debug soient actualisées
bool CarteDeplacement2009::mesureActualisee() {
	return _mesureOk;
}

// Lit une donnée de debug
std::int16_t CarteDeplacement2009::lireMesure(TypeMesure type) {
	return _mesure[type];
}

// Permet d'activer ou de désactiver certains types d'asservissement (distance, angle, roue droite, roue gauche)
void CarteDeplacement2009::activerAsservissement(TypeGrandeur type, bool active) {
	_ack = ACK_RIEN;

	envoyerMessage(this->make_trame(ASSERV_ON_OFF, type, active));
}


// Vérifie qu'un _ack ait été reçu
CarteDeplacement2009::TypeAck CarteDeplacement2009::ackRecu() {
	return _ack.exchange(ACK_RIEN);
}

void CarteDeplacement2009::activerEnvoiAuto(bool on_off) {
	_ack = ACK_RIEN;

	envoyerMessage(ENVOI_AUTO, on_off);
}

// Définit la variable 'param' (voir les define dans le .h) à la valeur 'val'
void CarteDeplacement2009::reglerParametre(uint8_t param, float val) {
	_ack = ACK_RIEN;

	Trame t = this->make_trame(SET_VARIABLE, param, val);
	envoyerMessage(t);
}

// Demande la valeur de la variable 'param' (voir les define dans le .h)
void CarteDeplacement2009::demanderParametre(uint8_t param) {
	_parametreOk = false;
	_ack = ACK_RIEN;

	envoyerMessage(GET_VARIABLE, param);
}

bool CarteDeplacement2009::parametreActualise() {
	return _parametreOk;
}

// Lit une donnée de debug
float CarteDeplacement2009::lireParametre() {
	return _parametre;
}

Duration CarteDeplacement2009::getPeriodeEnvoiAuto() {
	return PERIODE_ENVOI_AUTO;
}

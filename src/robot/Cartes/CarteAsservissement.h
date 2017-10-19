/*
 * CarteAsservissement.h
 *
 *  Created on: 23 févr. 2015
 *      Author: jerk
 */

#ifndef ROBOT_CARTES_CARTEASSERVISSEMENT_H_
#define ROBOT_CARTES_CARTEASSERVISSEMENT_H_

#include "Carte.h"
#include "CarteDeplacementCommun.h"

class CarteAsservissement : public Carte {
public:
	using Carte::Carte;

	enum TypeAck : uint8_t { ACK_RIEN, ACK_MESSAGE_EXECUTE = 'R', ACK_MESSAGE_IGNORE = 'I' };

	enum TypeDeplacement : uint8_t {
		DEPLACEMENT_RIEN,
		DEPLACEMENT_ARRETER = 0,
		DEPLACEMENT_TOURNER = 1,
		DEPLACEMENT_TOURNER_ABSOLU = 2,
		DEPLACEMENT_TOURNER_INFINI = 3,
		DEPLACEMENT_ARRET_URGENCE = 4,
	};

	enum TypeMesure : uint8_t {
		MESURE_POSITION = 0,
		MESURE_VITESSE_CONSIGNE = 1,
		MESURE_VITESSE_MESUREE = 2,
		MESURE_COMMANDE = 3
	};

	// Fait redemarrer la carte
	virtual void reset() = 0;

	// Actualise l'angle actuel du moteur
	virtual void actualiserAngle() = 0;

	// Verifie que l'angle actualise est arrive
	virtual bool angleActualise() = 0;

	// Retourne l'angle du moteur
	virtual Angle lireAngle() = 0;

	// Definit l'angle actuel du moteur
	virtual void definirAngle(Angle angle) = 0;

	// Indique si le déplacement est terminé ou pas
	virtual bool verifierDeplacementTermine() = 0;

	// Indique si la précision autour de la cible a été atteinte ou non
	virtual bool verifierPrecisionAtteinte() = 0;

	// Indique si le moteur est bloqué ou pas.
	virtual bool verifierBlocage() = 0;

	// S'arrêter aux coordonnées actuelles
	virtual void arreter() = 0;

	// Tourner de l'angle donné
	virtual void tourner(Angle angle) = 0;

	// Tourner jusqu'à avoir l'angle absolu donné
	virtual void tournerAbsolu(Angle angle) = 0;

	// Tourner à vitesse constante jusqu'à nouvel ordre
	virtual void tournerInfini(AngularSpeed vitesse, SensRotation sens) = 0;

	// Arret d'urgence
	virtual void arretUrgence(void) = 0;

	// Actualise les données de debug demandées
	virtual void demanderMesure() = 0;

	// Vérifie que les données de debug soient actualisées
	virtual bool mesureActualisee() = 0;

	// Lit une donnée de debug
	virtual short lireMesure(TypeMesure type) = 0;

	// Allume ou éteint la LED témoin
	virtual void allumerLed(bool led1, bool led2) = 0;

	// Permet d'activer ou de désactiver l'asservissement en angle
	virtual void activerAsservissementAngle(bool active) = 0;

	// Vérifie qu'un ack ait été reçu
	virtual TypeAck ackRecu() = 0;

	// Active l'envoi automatique des coordonnées du robot
	virtual void activerEnvoiAuto(bool on_off) = 0;

	// Définit la variable 'param' (voir les define dans le .h) à la valeur 'val'
	virtual void reglerParametre(uint8_t param, float val) = 0;

	// Demande à la carte d'envoyer la valeur d'un parametre
	virtual void demanderParametre(uint8_t param) = 0;

	// Signifie si le dernier parametre demande a ete recu ou pas
	virtual bool parametreActualise() = 0;

	// Retourne la valeur du dernier parametre recu
	virtual float lireParametre() = 0;

public:
	// ID des messages pour la carte de déplacement 2009
	enum IDMessages : uint8_t {
		RESET = 15,
		DETECT = 14,
		GET_VARIABLE = 13,
		MESURE = 12,
		TERMINE = 11,
		ANGLE = 10,
		BLOQUE = 9,
		LED = 8,
		ENVOI_AUTO = 7,
		ASSERV_ON_OFF = 6,
		ARRETURGENCE = 5, // Pas utilisé finalement : on fait avec COMMANDE = DEPLACEMENT_ARRET_URGENCE
		COMMANDE = 4,
		DEFINIR_ANGLE = 3,
		SET_VARIABLE = 2,
		ACK = 1,
		PING = 0
	};

	// ID des variables modifiables sur la carte
	enum TypeParametre {
		Carte_Fcy = 0,
		Carte_Tcy = 1,
		Carte_Te = 2,
		Carte_Fe = 3,
		Carte_Fpwm = 4,
		Carte_Tpwm = 5,
		Mecanique_EntreAxe = 6,
		Mecanique_DiametreRoueD = 7,
		Mecanique_DiametreRoueG = 8,
		Mecanique_PuissanceMoteurD = 9,
		Mecanique_PuissanceMoteurG = 10,
		Mecanique_PuissanceMaxD = 11,
		Mecanique_PuissanceMaxG = 12,
		Mecanique_PuissanceMaxLongitudinal = 13,
		Mecanique_PuissanceMaxAngulaire = 14,
		Mecanique_VitesseLongitudinaleMax = 15,
		Mecanique_VitesseAngulaireMax = 16,
		Mecanique_VitesseRoueDMax = 17,
		Mecanique_VitesseRoueGMax = 18,
		Mecanique_NombrePasD = 19,
		Mecanique_NombrePasG = 20,
		Etat_DistanceTotale = 21,
		Etat_DistanceRoueD = 22,
		Etat_DistanceRoueG = 23,
		Etat_AngleBrut = 24,
		Etat_AngleReel = 25,
		Etat_VitesseLongitudinale = 26,
		Etat_VitesseAngulaire = 27,
		Etat_VitesseRoueD = 28,
		Etat_VitesseRoueG = 29,
		Etat_X = 30,
		Etat_Y = 31,
		Etat_EnvoiAuto = 32,
		Longitudinal_Desire = 33,
		Longitudinal_Consigne = 34,
		Longitudinal_VitesseMax = 35,
		Longitudinal_VitesseDemande = 36,
		Longitudinal_VitesseConsigne = 37,
		Longitudinal_VitesseConsignePrecedente = 38,
		Longitudinal_Acceleration = 39,
		Longitudinal_Decceleration = 216,
		Longitudinal_Est_Active = 40,
		Angulaire_Desire = 41,
		Angulaire_Consigne = 42,
		Angulaire_VitesseMax = 43,
		Angulaire_VitesseDemande = 44,
		Angulaire_VitesseConsigne = 45,
		Angulaire_VitesseConsignePrecedente = 46,
		Angulaire_Acceleration = 47,
		Angulaire_Decceleration = 217,
		Angulaire_Est_Active = 48,
		RoueD_Desire = 49,
		RoueD_Consigne = 50,
		RoueD_VitesseMax = 51,
		RoueD_VitesseDemande = 52,
		RoueD_VitesseConsigne = 53,
		RoueD_VitesseConsignePrecedente = 54,
		RoueD_Acceleration = 55,
		RoueD_Decceleration = 218,
		RoueD_Est_Active = 56,
		RoueG_Desire = 57,
		RoueG_Consigne = 58,
		RoueG_VitesseMax = 59,
		RoueG_VitesseDemande = 60,
		RoueG_VitesseConsigne = 61,
		RoueG_VitesseConsignePrecedente = 62,
		RoueG_Acceleration = 63,
		RoueG_Decceleration = 219,
		RoueG_Est_Active = 64,
		PidPositionLongitudinale_Erreur = 65,
		PidPositionLongitudinale_ErreurPrecedente = 66,
		PidPositionLongitudinale_IntegraleErreur = 67,
		PidPositionLongitudinale_DeriveErreur = 68,
		PidPositionLongitudinale_CommandeProportionelle = 69,
		PidPositionLongitudinale_CommandeIntegrale = 70,
		PidPositionLongitudinale_CommandeDerive = 71,
		PidPositionLongitudinale_Kp = 72,
		PidPositionLongitudinale_Ki = 73,
		PidPositionLongitudinale_Kd = 74,
		PidPositionLongitudinale_KpArret = 75,
		PidPositionLongitudinale_KiArret = 76,
		PidPositionLongitudinale_KdArret = 77,
		PidPositionLongitudinale_KpRoule = 78,
		PidPositionLongitudinale_KiRoule = 79,
		PidPositionLongitudinale_KdRoule = 80,
		PidPositionLongitudinale_Commande = 81,
		PidPositionAngulaire_Erreur = 82,
		PidPositionAngulaire_ErreurPrecedente = 83,
		PidPositionAngulaire_IntegraleErreur = 84,
		PidPositionAngulaire_DeriveErreur = 85,
		PidPositionAngulaire_CommandeProportionelle = 86,
		PidPositionAngulaire_CommandeIntegrale = 87,
		PidPositionAngulaire_CommandeDerive = 88,
		PidPositionAngulaire_Kp = 89,
		PidPositionAngulaire_Ki = 90,
		PidPositionAngulaire_Kd = 91,
		PidPositionAngulaire_KpArret = 92,
		PidPositionAngulaire_KiArret = 93,
		PidPositionAngulaire_KdArret = 94,
		PidPositionAngulaire_KpRoule = 95,
		PidPositionAngulaire_KiRoule = 96,
		PidPositionAngulaire_KdRoule = 97,
		PidPositionAngulaire_Commande = 98,
		PidVitesseLongitudinale_Erreur = 99,
		PidVitesseLongitudinale_ErreurPrecedente = 100,
		PidVitesseLongitudinale_IntegraleErreur = 101,
		PidVitesseLongitudinale_DeriveErreur = 102,
		PidVitesseLongitudinale_CommandeProportionelle = 103,
		PidVitesseLongitudinale_CommandeIntegrale = 104,
		PidVitesseLongitudinale_CommandeDerive = 105,
		PidVitesseLongitudinale_Kp = 106,
		PidVitesseLongitudinale_Ki = 107,
		PidVitesseLongitudinale_Kd = 108,
		PidVitesseLongitudinale_KpArret = 109,
		PidVitesseLongitudinale_KiArret = 110,
		PidVitesseLongitudinale_KdArret = 111,
		PidVitesseLongitudinale_KpRoule = 112,
		PidVitesseLongitudinale_KiRoule = 113,
		PidVitesseLongitudinale_KdRoule = 114,
		PidVitesseLongitudinale_Commande = 115,
		PidVitesseAngulaire_Erreur = 116,
		PidVitesseAngulaire_ErreurPrecedente = 117,
		PidVitesseAngulaire_IntegraleErreur = 118,
		PidVitesseAngulaire_DeriveErreur = 119,
		PidVitesseAngulaire_CommandeProportionelle = 120,
		PidVitesseAngulaire_CommandeIntegrale = 121,
		PidVitesseAngulaire_CommandeDerive = 122,
		PidVitesseAngulaire_Kp = 123,
		PidVitesseAngulaire_Ki = 124,
		PidVitesseAngulaire_Kd = 125,
		PidVitesseAngulaire_KpArret = 126,
		PidVitesseAngulaire_KiArret = 127,
		PidVitesseAngulaire_KdArret = 128,
		PidVitesseAngulaire_KpRoule = 129,
		PidVitesseAngulaire_KiRoule = 130,
		PidVitesseAngulaire_KdRoule = 131,
		PidVitesseAngulaire_Commande = 132,
		PidPositionD_Erreur = 133,
		PidPositionD_ErreurPrecedente = 134,
		PidPositionD_IntegraleErreur = 135,
		PidPositionD_DeriveErreur = 136,
		PidPositionD_CommandeProportionelle = 137,
		PidPositionD_CommandeIntegrale = 138,
		PidPositionD_CommandeDerive = 139,
		PidPositionD_Kp = 140,
		PidPositionD_Ki = 141,
		PidPositionD_Kd = 142,
		PidPositionD_KpArret = 143,
		PidPositionD_KiArret = 144,
		PidPositionD_KdArret = 145,
		PidPositionD_KpRoule = 146,
		PidPositionD_KiRoule = 147,
		PidPositionD_KdRoule = 148,
		PidPositionD_Commande = 149,
		PidPositionG_Erreur = 150,
		PidPositionG_ErreurPrecedente = 151,
		PidPositionG_IntegraleErreur = 152,
		PidPositionG_DeriveErreur = 153,
		PidPositionG_CommandeProportionelle = 154,
		PidPositionG_CommandeIntegrale = 155,
		PidPositionG_CommandeDerive = 156,
		PidPositionG_Kp = 157,
		PidPositionG_Ki = 158,
		PidPositionG_Kd = 159,
		PidPositionG_KpArret = 160,
		PidPositionG_KiArret = 161,
		PidPositionG_KdArret = 162,
		PidPositionG_KpRoule = 163,
		PidPositionG_KiRoule = 164,
		PidPositionG_KdRoule = 165,
		PidPositionG_Commande = 166,
		PidVitesseD_Erreur = 167,
		PidVitesseD_ErreurPrecedente = 168,
		PidVitesseD_IntegraleErreur = 169,
		PidVitesseD_DeriveErreur = 170,
		PidVitesseD_CommandeProportionelle = 171,
		PidVitesseD_CommandeIntegrale = 172,
		PidVitesseD_CommandeDerive = 173,
		PidVitesseD_Kp = 174,
		PidVitesseD_Ki = 175,
		PidVitesseD_Kd = 176,
		PidVitesseD_KpArret = 177,
		PidVitesseD_KiArret = 178,
		PidVitesseD_KdArret = 179,
		PidVitesseD_KpRoule = 180,
		PidVitesseD_KiRoule = 181,
		PidVitesseD_KdRoule = 182,
		PidVitesseD_Commande = 183,
		PidVitesseG_Erreur = 184,
		PidVitesseG_ErreurPrecedente = 185,
		PidVitesseG_IntegraleErreur = 186,
		PidVitesseG_DeriveErreur = 187,
		PidVitesseG_CommandeProportionelle = 188,
		PidVitesseG_CommandeIntegrale = 189,
		PidVitesseG_CommandeDerive = 190,
		PidVitesseG_Kp = 191,
		PidVitesseG_Ki = 192,
		PidVitesseG_Kd = 193,
		PidVitesseG_KpArret = 194,
		PidVitesseG_KiArret = 195,
		PidVitesseG_KdArret = 196,
		PidVitesseG_KpRoule = 197,
		PidVitesseG_KiRoule = 198,
		PidVitesseG_KdRoule = 199,
		PidVitesseG_Commande = 200,
		MouvementActuel_DistanceDemande = 201,
		MouvementActuel_AngleDemande = 202,
		MouvementActuel_DistanceDroiteDemande = 203,
		MouvementActuel_DistanceGaucheDemande = 204,
		MouvementActuel_Xc = 205,
		MouvementActuel_Yc = 206,
		MouvementActuel_PrecisionLongitudinale = 207,
		MouvementActuel_PrecisionAngulaire = 208,
		MouvementActuel_Type = 209,
		MouvementActuel_EnAvant = 210,
		MouvementActuel_Etape = 211,
		MouvementActuel_Termine = 212,
		MouvementActuel_TermineEnvoye = 213,
		MouvementActuel_TempsBlocage = 214,
		MouvementActuel_ConfigAsserv = 215
	};
};


#endif /* ROBOT_CARTES_CARTEASSERVISSEMENT_H_ */

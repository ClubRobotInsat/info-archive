// IDCartes2009.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

enum IdCarte
{
	// ID Definitifs
	ID_CARTE_CAN_USB = 0,
	ID_CARTE_DEPLACEMENT = 1,
	ID_CARTE_DETECT_ADV = 2,
	ID_CARTE_ASSERVISSEMENT_ASCENCEUR = 3,
	ID_CARTE_ASSERVISSEMENT_TAPIS = 4,
	ID_CARTE_SERVOS_NOVA_RAMASSAGE = 5,
	ID_CARTE_SERVOS_NOVA_TRI = 6,
	ID_CARTE_TELEMETRE_EVITEMENT	= 7,
	ID_CARTE_TELEMETRE_DETECT_PALETS	= 8,
	ID_CARTE_COLORIMETRE = 9,
	ID_CARTE_ALIM = 10,
	ID_CARTE_CONTACTEURS = 11,	// tirette, couleur equipe, recalage ascenceur,
								// recalage detect adv, recalage robot avant / arriere
	ID_CARTE_CONTACTEURS_RESERVE = 12, // Au cas ou
	ID_CARTE_BARRIERE_IR = 13,
	ID_CARTE_BALISE_XBEE = 14,
	ID_BROADCAST = 15,
	ID_CARTE_DEBUG       = 17,
	ID_CARTE_LCD       = 18
};

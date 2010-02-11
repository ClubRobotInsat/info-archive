// IDCartes2010.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

enum IdCarte
{
	// ID Definitifs
	ID_CARTE_CAN_USB = 0,
	ID_CARTE_DEPLACEMENT = 1,
	ID_CARTE_CONTACTEURS = 11,	// tirette, couleur equipe, recalage ascenceur,
								// recalage detect adv, recalage robot avant / arriere
	ID_BROADCAST = 15,
	ID_CARTE_DEBUG_DESSIN = 16,
	ID_CARTE_DEBUG = 17,
	ID_CARTE_LCD = 18,

	ID_CARTE_BALISE_INERTIELLE = 2,

	// VERSION 5 SERVOS PAR CARTE (rev 1800)
	// ID_CARTE_SERVOS_NOVA_RAMASSAGE = 3, 	// servos pinces + fourches
	// ID_CARTE_SERVOS_NOVA_DEPOSE = 4, 	// porte + plancher

	// VERSION 3 SERVOS PAR CARTE
	ID_CARTE_SERVOS_NOVA_AVANT = 3, 		// servos pinces + plancher
	ID_CARTE_SERVOS_NOVA_ARRIERE = 4, 		// fourches + porte

	ID_CARTE_ASSERVISSEMENT_PINCE = 5
};

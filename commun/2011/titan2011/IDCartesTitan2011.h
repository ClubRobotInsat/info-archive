// IDCartes2011.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

#ifndef IDCARTES_TITAN_2011_H
#define IDCARTES_TITAN_2011_H

namespace IDCartesTitan2011
{
enum IdCarte
{
	ID_CARTE_CAN_USB                          = 0,
	ID_CARTE_DEPLACEMENT                      = 1,
	ID_CARTE_CONTACTEURS_BRAS                 = 2,
	ID_CARTE_CONTACTEURS_DEPART               = 4,
	ID_CARTE_SERVOS_NOVA_ASCENSEUR            = 5,
	ID_CARTE_ASSERVISSEMENT_ASCENSEUR         = 6,
	ID_CARTE_ASSERVISSEMENT_BRAS_GAUCHE       = 7,
	ID_CARTE_ASSERVISSEMENT_BRAS_DROIT        = 8,
	ID_CARTE_CODES_BARRES					  = 9,
	ID_CARTE_MAT_LASER                        = 10,
	ID_CARTE_ALIMENTATION                     = 11,
	ID_CARTE_DEBUG_DESSIN                     = 12,
	ID_CARTE_DEBUG                            = 13,
	ID_CARTE_LCD                              = 14
};

}

#endif

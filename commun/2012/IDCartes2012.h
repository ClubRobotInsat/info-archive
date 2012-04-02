// IDCartes2012.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

#ifndef IDCARTES_2012_H
#define IDCARTES_2012_H

namespace IDCartes2012
{
	enum IdCarte
	{
                ID_CARTE_CAN_USB                                = 0,
                ID_CARTE_DEPLACEMENT                            = 1,
		ID_CARTE_ASSERVISSEMENT_LINGOT_TOTEM 		= 2,
		ID_CARTE_ASSERVISSEMENT_MOISSONNEUSE		= 3,
                ID_CARTE_ASSERVISSEMENT_COFFRE                  = 4,
                ID_CARTE_CONTACTEURS_DEPART                     = 5,
                ID_CARTE_SERVOS_NOVA_AVANT_DROIT                = 6,
                ID_CARTE_SERVOS_NOVA_AVANT_GAUCHE               = 7,
                ID_CARTE_SERVOS_AUTRES                          = 8,
                ID_CARTE_POS_ADV                                = 11,
                ID_CARTE_ALIMENTATION                           = 12,
		ID_CARTE_DEBUG_DESSIN                     	= 13,
		ID_CARTE_DEBUG                            	= 14
	};
}
#endif

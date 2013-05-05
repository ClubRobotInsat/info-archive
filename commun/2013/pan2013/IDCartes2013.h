// IDCartes2012.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

#ifndef IDCARTES_2013_PAN_H
#define IDCARTES_2013_PAN_H

namespace IDCartes2013
{
	enum IdCarte
	{
        ID_CARTE_CAN_USB                    = 0,
        ID_CARTE_DEPLACEMENT                = 1,
        ID_CARTE_CONTACTEURS                = 3,
        ID_CARTE_SERVOS                     = 4,
        ID_CARTE_EVITEMENT                  = 8,
        ID_CARTE_DEBUG_DESSIN				= 6,
        ID_CARTE_IHM						= 7,
        ID_CARTE_POMPE						= 9
	};
}
#endif

// IDCartes2010.h - les IDs des cartes
// En-tête utilisé par le simu et par l'IE

#ifndef IDCARTES_CHRONOS_2014
#define IDCARTES_CHRONOS_2014

namespace IDCartesChronos2014
{
	enum IdCarteChronos
	{
		ID_CARTE_CAN_USB                                = 0,
		ID_CARTE_DEPLACEMENT                            = 1,
		ID_CARTE_ASSERVISSEMENT_ASCENSEUR_GAUCHE 		= 3,
		ID_CARTE_ASSERVISSEMENT_ASCENSEUR_DROIT			= 2,
		ID_CARTE_SERVOS_ASCENSEUR		                = 4,
		ID_CARTE_SERVOS_BRAS			                = 5,
		ID_CARTE_CONTACTEURS_ASC		                = 6,
		ID_CARTE_IHM				                    = 7,
		ID_CARTE_EVITEMENT                              = 8,
		ID_CARTE_PNEUMATIQUE                            = 9,
		ID_CARTE_DEBUG_DESSIN                     		= 10,
		//ID_CARTE_DEBUG                            		= 11
	};
}
#endif

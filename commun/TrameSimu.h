/*
 * TrameSimu.h
 *
 *  Created on: 30 avr. 2015
 *      Author: seydoux
 */

#ifndef COMMUN_TRAMESIMU_H_
#define COMMUN_TRAMESIMU_H_

/**
 * Big-ass trame, moins contrainte que les trames qu'on envoie sur le CAN.
 * Ces trames serviront à la communication spécifique entre simu et robot :
 * Déplacement d'objets, affichage de débug...
 */
class TrameSimu {
public:
	//static const
	TrameSimu();
	virtual ~TrameSimu();
};

#endif /* COMMUN_TRAMESIMU_H_ */

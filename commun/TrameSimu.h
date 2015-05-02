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
	// Nombre d'octets en tête de trame dédiés à la commande
	static const int NB_OCTETS_CMD;
	TrameSimu(uint8_t cmd, long trameSize);

	virtual ~TrameSimu();
private:
	uint8_t _cmd;
	long _trameSize;

};

#endif /* COMMUN_TRAMESIMU_H_ */

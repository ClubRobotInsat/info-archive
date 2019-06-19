/*
 * CarteCAN_USB.h
 *
 *  Created on: 8 mai 2014
 *      Author: seydoux
 */

#ifndef CARTECAN_USB_H_
#define CARTECAN_USB_H_

#include "Carte.h"
#include <cstring>

class CarteCAN_USB : public Carte {
public:
	CarteCAN_USB(Commun::CAN&, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	// En début de match, il faut systématiquement synchroniser le numéro de séquence
	// de l'info et de l'élec, donc on impose le numéro de l'élec à 0
	void reinitialiserNumSequence();
	bool isInitialiseNumSequence();

private:
	std::atomic_bool _init;
};


#endif /* CARTECAN_USB_H_ */

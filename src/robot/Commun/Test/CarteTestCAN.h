//
//  CarteTestCAN.h
//  Club Robot
//
//  Created by Rémi on 31/01/2015.
//

#ifndef __Club_Robot__CarteTestCAN__
#define __Club_Robot__CarteTestCAN__

#include "../../Cartes/Carte.h"
#include <functional>

class CarteTestCAN : public Carte {
public:
	CarteTestCAN(Commun::CAN& can, uint8_t id);

	virtual void traiterMessage(Trame const& message) override;

	void setListener(std::function<void(Trame)> listener) {
		_listener = listener;
	}

private:
	std::function<void(Trame)> _listener;
};

#endif /* defined(__Club_Robot__CarteTestCAN__) */

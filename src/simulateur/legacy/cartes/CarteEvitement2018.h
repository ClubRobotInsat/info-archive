#ifndef ROOT_CARTEEVITEMENT2018_H
#define ROOT_CARTEEVITEMENT2018_H

#include "Carte.h"

class CarteEvitement2018 : public Carte {
public:
	CarteEvitement2018(unsigned char id, RobotLogic& robot);

	virtual ~CarteEvitement2018() = default;

	virtual void traiterTrame(Trame const&) override;

	std::pair<bool, Angle> lirePositionAdversaire() {
		return std::make_pair(false, 0_deg);
	};

	void pong();
};

#endif // ROOT_CARTEEVITEMENT2018_H

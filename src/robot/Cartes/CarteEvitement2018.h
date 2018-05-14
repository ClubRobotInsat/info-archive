#ifndef CARTE_EVITEMENT_2018_H
#define CARTE_EVITEMENT_2018_H

#include "Carte.h"

class CarteEvitement2018 : public Carte {
public:
	CarteEvitement2018(Commun::CAN& can, uint8_t ID);

	void traiterMessage(Trame const& message) override;

	std::pair<bool, Angle> lirePositionAdversaire();

	enum Commande {
		ACTUALISATION = 5, // 6 octets en RECEPTION : CMD | adv_present<uint8_t> | angle_deg<float>
	};

private:
	std::atomic<Angle> _last_detected_angle;

	std::atomic<StopWatch> _time_since_last_detected;
};

#endif // CARTE_EVITEMENT_2018_H

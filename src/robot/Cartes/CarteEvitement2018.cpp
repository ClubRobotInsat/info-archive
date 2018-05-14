#include "CarteEvitement2018.h"

CarteEvitement2018::CarteEvitement2018(Commun::CAN& can, uint8_t id)
        : Carte(can, id), _last_detected_angle(0_deg), _time_since_last_detected(StopWatch()) {}

void CarteEvitement2018::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case ACTUALISATION:
			if(message.getNbDonnees() == 5) {
				bloquerVariables();
				bool detected = static_cast<bool>(message.getDonnee<uint8_t>(0));

				if(detected) {
					_time_since_last_detected = StopWatch();
				}
				// logDebug("Carte évitement : ", message, "; detected ? ", detected);

				debloquerVariables();
				break;
			}
		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

std::pair<bool, Angle> CarteEvitement2018::lirePositionAdversaire() {
	std::lock_guard<std::mutex> lk(_mutexVariables);
	return std::make_pair<bool, Angle>(_time_since_last_detected.load().getElapsedTime() < 500_ms, _last_detected_angle.load());
}

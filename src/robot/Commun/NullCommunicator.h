//
//  NullCommunicator.h
//  Club Robot
//
//  Created by Rémi on 21/03/2016.
//

#ifndef NullCommunicator_h
#define NullCommunicator_h

#include "Serie.h"
#include <thread>

namespace Commun {
	class NullCommunicator : public Serie {
	public:
		NullCommunicator() = default;
		~NullCommunicator() = default;

		virtual void ecrireOctets(uint8_t const*, std::size_t) override {}

		virtual void lireOctets(uint8_t* octets, std::size_t nombre) override {
			while(nombre > 0) {
				--nombre;
				octets[nombre] = 0;
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}

		bool estConnecte() {
			return true;
		}
	};
}


#endif /* NullCommunicator_h */

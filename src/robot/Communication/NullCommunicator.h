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

namespace Communication {
	class NullCommunicator : public Serie {
	public:
		NullCommunicator();
		~NullCommunicator();

		virtual void ecrireOctets(uint8_t const*, std::size_t) override;

		virtual void lireOctets(uint8_t* octets, std::size_t nombre) override;

		bool estConnecte() {
			return true;
		}
	};
} // namespace Communication


#endif /* NullCommunicator_h */

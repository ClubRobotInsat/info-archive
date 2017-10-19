#ifndef WRAPPER_CARTES_2008_H_
#define WRAPPER_CARTES_2008_H_

// WrapperCartes2008.h - ici sont regroupée les pour boost python des cartes 2008.
#include <boost/python.hpp>

#include "CarteAsservissement2008.h"
#include "CarteBalisesIR2008.h"
#include "CarteColorimetres2008.h"
#include "CarteLcd2008.h"
#include "CarteLed2008.h"
#include "CarteMoteursNonAsservis2008.h"
#include "CarteTelemetreIR2008.h"
#include "CarteTriangulation2008.h"

using namespace boost::python;

class CarteAsservissement2008Wrap : public CarteAsservissement2008, public wrapper<CarteAsservissement2008> {
public:
	virtual void traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) override {
		if(override traiterMessage = this->get_override("traiterMessage"))
			traiterMessage(cmd, taille, donnees);
		CarteAsservissement2008::traiterMessage(cmd, taille, donnees);
	}

	void default_traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) {
		this->CarteAsservissement2008::traiterMessage(cmd, taille, donnees);
	}
};

class CarteTelemetreIR2008Wrap : public CarteTelemetreIR2008, public wrapper<CarteTelemetreIR2008> {
public:
	virtual void traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) override {
		if(override traiterMessage = this->get_override("traiterMessage"))
			traiterMessage(cmd, taille, donnees);
		CarteTelemetreIR2008::traiterMessage(cmd, taille, donnees);
	}

	void default_traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) {
		this->CarteTelemetreIR2008::traiterMessage(cmd, taille, donnees);
	}
};

#endif // WRAPPER_CARTES_2008_H_

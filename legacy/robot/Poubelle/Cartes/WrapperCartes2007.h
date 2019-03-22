#ifndef WRAPPER_CARTES_2007_H_
#define WRAPPER_CARTES_2007_H_

// WrapperCartes2007.h - ici sont regroupée les pour boost python des cartes 2007.
#include <boost/python.hpp>

#include "CarteActionneurs2007.h"
#include "CarteBalises2007.h"
#include "CarteBarrieres2007.h"
#include "CarteContacteurs2007.h"
#include "CarteConvertisseurs2007.h"
#include "CarteDeplacement2007.h"
#include "CarteServos2007.h"
#include "POBEye2007.h"

using namespace boost::python;

class CarteActionneurs2007Wrap : public CarteActionneurs2007, public wrapper<CarteActionneurs2007> {
public:
	virtual void traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) override {
		if(override traiterMessage = this->get_override("traiterMessage"))
			traiterMessage(cmd, taille, donnees);
		CarteActionneurs2007::traiterMessage(cmd, taille, donnees);
	}

	void default_traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) {
		this->CarteActionneurs2007::traiterMessage(cmd, taille, donnees);
	}
};

#endif // WRAPPER_CARTES_2007_H_

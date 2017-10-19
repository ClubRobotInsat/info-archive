#ifndef WRAPPER_CARTES_2009_H_
#define WRAPPER_CARTES_2009_H_

// WrapperCartes2009.h - ici sont regroupée les pour boost python des cartes 2009.
#include <boost/python.hpp>

#include "CarteAlimentation2010.h"
#include "CarteAsservissement2009.h"
#include "CarteCodesBarres2011.h"
#include "CarteContacteurs2007.h"
#include "CarteDebug2009.h"
#include "CarteDeplacement2009.h"
#include "CarteDetectAdv2009.h"
#include "CarteEvitement2012.h"
#include "CarteIHM.h"
#include "CarteIO2014.h"
#include "CarteLcd2009.h"
#include "CartePneumatique2014.h"
#include "CarteServosNova2017.h"
#include "CarteTelemetres2009.h"

using namespace boost::python;

class CarteLcd2009Wrap : public CarteLcd2009, public wrapper<CarteLcd2009> {
public:
	virtual void traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) {
		if(override traiterMessage = this->get_override("traiterMessage"))
			traiterMessage(cmd, taille, donnees);
		CarteLcd2009::traiterMessage(cmd, taille, donnees);
	}

	void default_traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) {
		this->CarteLcd2009::traiterMessage(cmd, taille, donnees);
	}
};


#endif // WRAPPER_CARTES_2009_H_

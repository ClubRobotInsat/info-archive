/************************************************************
 * CarteTriangulation2008.cpp *
 ************************************************************/

#include "CarteTriangulation2008.h"

void CarteTriangulation2008::traiterMessage(Trame const& message) {
	switch(message.getCmd()) {
		case 0x09_b: // Reception de coordonnees
			if(message.getNbDonnees() == 4) {
				bloquerVariables();
				_x = message.getDonnee<std::int16_t>(0);
				_y = message.getDonnee<std::int16_t>(2);
				debloquerVariables();
			}
			break;

		default:
			this->Carte::traiterMessage(message);
			break;
	}
}

// Récupère des coordonnées
void CarteTriangulation2008::recupereCoordonnees(std::int16_t* x, std::int16_t* y) {
	bloquerVariables();
	*x = _x;
	*y = _y;
	debloquerVariables();
}

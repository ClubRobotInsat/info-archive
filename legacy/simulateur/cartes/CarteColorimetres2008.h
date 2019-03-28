#ifndef CARTECOLORIMETRES2008_H_
#define CARTECOLORIMETRES2008_H_

#include "../simulateur/Robot/Carte.h"
#include "IColorimetres.h"

class CarteColorimetres2008 : public Carte {
public:
	static const unsigned char NBR_CAPTEURS;

	CarteColorimetres2008(unsigned char id, Robot& robot, IColorimetres& iColorimetres);

	virtual ~CarteColorimetres2008();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi l'etat des contacteurs...
	void envoyerEtatColorimetres(unsigned char num);

	// erreur le numero du colorimetre est trop grand
	class ErreurNumCapteurs : public std::runtime_error {
	public:
		ErreurNumCapteurs(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " + to_string((short)NBR_CAPTEURS)) {}
	};

private:
	IColorimetres& _iColorimetres;
};

#endif /*CARTECOLORIMETRES2008_H_*/

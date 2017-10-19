#ifndef CARTESERVOS2007_H_
#define CARTESERVOS2007_H_

#include "../simulateur/Robot/Carte.h"
#include "IServos.h"

class CarteServos2007 : public Carte {
public:
	static const unsigned char SERVOS_NBR;

	// constructeur
	CarteServos2007(unsigned char id, Robot& robot, IServos& iServos);

	// destructeur
	virtual ~CarteServos2007();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi reponce a un ping
	virtual void envoyerPong();

	// Envoi acquittement
	virtual void envoyerAcquittementAngle(unsigned char numServo);

	// erreur le numero du servo est trop grand
	class ErreurNumServos : public std::runtime_error {
	public:
		ErreurNumServos(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " de servo est superieur a " +
		                             to_string((short)SERVOS_NBR)) {}
	};

private:
	IServos& _iServos;
};

#endif /*CARTESERVOS2007_H_*/

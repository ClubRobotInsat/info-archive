// CarteBalises2007.h

#ifndef CARTEBALISES2007_H_
#define CARTEBALISES2007_H_

#include "../simulateur/Robot/Carte.h"
#include "../simulateur/Robot/Robot.h"
#include "IBalise.h"

class CarteBalises2007 : public Carte {
public:
	const static char NBR_BALISES = 4;

	CarteBalises2007(unsigned char id, Robot& robot);

	// associer une des balises
	void setBalise(unsigned char numero, const IBalise* balise);

	virtual ~CarteBalises2007();

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// envoie une trame toutes les 200 ms
	virtual void update(Duration elapsed) override;

	class ErreurNumBaliseInvalise : public std::runtime_error {
	public:
		ErreurNumBaliseInvalise(unsigned char num)
		        : std::runtime_error("Numero " + to_string((short)num) + " est un numero de balise invalide") {}
	};

private:
	// pointeurs sur des objets 3D qui joue le role de balise
	// attention une balise peut etre nulle (non utilise)
	const IBalise* _balises[NBR_BALISES];

	void pong();
	void etatRecepteurs();

	// allumer un recepteur dans la trame corespondante
	void setOnRecepteur(unsigned char numBalise, unsigned char numRecepteur, Trame& trame1, Trame& trame2);
};

#endif /*CARTEBALISES2007_H_*/

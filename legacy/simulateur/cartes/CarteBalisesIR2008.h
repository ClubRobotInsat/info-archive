// CarteBalises2007.h

#ifndef CARTEBALISES_IR_2008_H_
#define CARTEBALISES_IR_2008_H_

#include "../simulateur/Robot/Carte.h"
#include "../simulateur/Robot/Robot.h"
#include "IBalise.h"

class CarteBalisesIR2008 : public Carte {
public:
	CarteBalisesIR2008(unsigned char id, Robot& robot);

	// associer la balise
	void setBalise(const IBalise* balise);

	virtual ~CarteBalisesIR2008();

	// Traite les donnees fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// envoie une trame toutes les 200 ms
	virtual void update(Duration elapsed) override;

private:
	// pointeurs sur des objets 3D qui joue le role de balise
	// attention une balise peut etre nulle (non utilise)
	const IBalise* _iBalise;

	bool _envoiAuto;

	void pong();
	void etatRecepteurs();

	// allumer un recepteur dans la trame corespondante
	void setOnRecepteur(unsigned char numRecepteur, Trame& trame);
};

#endif /*CARTEBALISES2007_H_*/

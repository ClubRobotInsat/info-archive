// CarteContacteurs2007.h

#ifndef CARTEBARRIERE2008_H_
#define CARTEBARRIERE2008_H_

#include "../simulateur/Robot/Carte.h"
#include "IBarrieres.h"

#define NOMBRE_BARRIERE 2 // doit etre inferieur ou egal a 8 car tout est fait en unsigned char.

class CarteBarriere2008 : public Carte {
public:
	CarteBarriere2008(unsigned char id, Robot& robot, IBarrieres& iBarrieres);

	virtual ~CarteBarriere2008();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// Envoi l'etat des barrieres...
	void envoyerEtatBarrieres();

	// verifie si les barrieres on ete modifier et envoie une trame si besoin
	virtual void update(Duration elapsed) override;

	// erreur le numero du contecteur est trop grand
	class ErreurNumBarriere : public std::runtime_error {
	public:
		ErreurNumBarriere(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " est supperieur a " + to_string((short)NOMBRE_BARRIERE)) {
		}
	};

	// erreur le Nombre de contecteur est trop grand
	class ErreurNbrBarriere : public std::runtime_error {
	public:
		ErreurNbrBarriere()
		        : std::runtime_error("le nombre de barrieres (" + to_string((short)NOMBRE_BARRIERE) +
		                             ") est trop grand pour etre stocker sur un char ") {}
	};

private:
	bool _barrieres[NOMBRE_BARRIERE];
	IBarrieres& _iBarrieres;
};

#endif /*CARTEBARRIERE2008_H_*/

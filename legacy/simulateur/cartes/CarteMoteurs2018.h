#ifndef CARTEMOTEURSNONASSERVIS2008_H_
#define CARTEMOTEURSNONASSERVIS2008_H_

#include "Carte.h"
#include "IMoteurs.h"

class CarteMoteurs2018 : public Carte {
public:
	static const unsigned char NBR_MAX_MOTEUR;

public:
	// constructeur
	CarteMoteurs2018(unsigned char id, RobotLogic& robot, IMoteurs& iMoteurs);

	// destructeur
	virtual ~CarteMoteurs2018() = default;

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// envoyer la reponce a un ping
	virtual void envoyerPong();

	// erreur le numero du moteur est trop grand
	class ErreurNumMoteur : public std::runtime_error {
	public:
		ErreurNumMoteur(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) +
		                             " du moteur non asservis est supperieur a " + to_string((short)NBR_MAX_MOTEUR)) {}
	};

private:
	IMoteurs& _iMoteurs;
};

#endif /*CARTEMOTEURSNONASSERVIS2008_H_*/

#ifndef CARTEMOTEURSNONASSERVIS2008_H_
#define CARTEMOTEURSNONASSERVIS2008_H_

#include "../simulateur/Robot/Carte.h"
#include "IMoteursNonAsservis.h"

class CarteMoteursNonAsservis2008 : public Carte {
public:
	static const unsigned char MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR;
	static const unsigned char MOTEUR_NON_ASSERVIS_PUISSANCE_MAX;

public:
	// constructeur
	CarteMoteursNonAsservis2008(unsigned char id, Robot& robot, IMoteursNonAsservis& iMoteursNonAsservis);

	// destructeur
	virtual ~CarteMoteursNonAsservis2008();

	// Traite les données fournies par l'IA
	virtual void traiterTrame(Trame const&) override;

	// envoyer la reponce a un ping
	virtual void envoyerPong();

	// Envoi l'acquittement de la puissance moteur atteinte
	virtual void envoyerAcquittementPuissance(unsigned char numMoteur);

	// erreur le numero du moteur est trop grand
	class ErreurNumMoteur : public std::runtime_error {
	public:
		ErreurNumMoteur(unsigned char num)
		        : std::runtime_error("le numero " + to_string((short)num) + " du moteur non asservis est supperieur a " +
		                             to_string((short)MOTEUR_NON_ASSERVIS_NBR_MAX_MOTEUR)) {}
	};

	// erreur la puissance du moteur est trop grand
	class ErreurPuissanceMoteur : public std::runtime_error {
	public:
		ErreurPuissanceMoteur(char puissance)
		        : std::runtime_error("la puissance " + to_string((short)puissance) + " du moteur non asservis est supperieur a " +
		                             to_string((short)MOTEUR_NON_ASSERVIS_PUISSANCE_MAX)) {}
	};

private:
	IMoteursNonAsservis& _iMoteursNonAsservis;
};

#endif /*CARTEMOTEURSNONASSERVIS2008_H_*/

// IPneumatique.h

#ifndef IPNEUMATIQUE_H_
#define IPNEUMATIQUE_H_

class IPneumatique {
public:
	virtual ~IPneumatique() {}

	// activation du pneumatique
	virtual void setActivation(unsigned char idCarte) = 0;

	virtual void setDesactivation(unsigned char idCarte) = 0;

	// TRUE si activé ; FALSE si désactivé
	virtual bool isActivated(unsigned char idCarte) = 0;
};

#endif /*IPNEUMATIQUE_H_*/

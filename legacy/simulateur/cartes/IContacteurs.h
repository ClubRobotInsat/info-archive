// IContacteurs.h

#ifndef ICONTACTEURS_H_
#define ICONTACTEURS_H_

class IContacteurs {
public:
	virtual ~IContacteurs() {}

	// Accesseur sur l'etat des contacteurs
	virtual bool getEtatContact(unsigned char numeroContact) const = 0;
};

#endif /*ICONTACTEURS_H_*/

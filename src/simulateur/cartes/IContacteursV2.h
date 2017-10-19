// IContacteurs.h

#ifndef ICONTACTEURS_V2_H_
#define ICONTACTEURS_V2_H_

class IContacteursV2 {
public:
	virtual ~IContacteursV2() {}

	// Accesseur sur l'etat des contacteurs
	virtual bool getContactorState(unsigned char numeroContact, unsigned char idCarte) const = 0;

	virtual void setContactorState(unsigned char numeroContact, unsigned char idCarte) = 0;
};

#endif /*ICONTACTEURS_H_*/

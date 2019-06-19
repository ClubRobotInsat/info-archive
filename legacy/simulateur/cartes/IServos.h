// IServos.h

#ifndef ISERVOS_H_
#define ISERVOS_H_

class IServos {
public:
	virtual ~IServos() {}

	// modifier la valeur d'un servos moteur
	virtual void setAngleServo(unsigned char idCarte, unsigned char numServo, unsigned char angle) = 0;
};

#endif /*ISERVOS_H_*/

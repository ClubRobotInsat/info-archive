//
// Created by paul on 19/04/16.
//

#ifndef ROOT_IVANNE_H_H
#define ROOT_IVANNE_H_H

class IVanne {
public:
	virtual ~IVanne() {}

	virtual void setValveOn(unsigned char idVanne) = 0;
	virtual void setValveOff(unsigned char idVanne) = 0;

	// True si vanne on, false si vanne off
	virtual bool getValveStatus(unsigned char idVanne) const = 0;
	virtual bool getAllValveStatus() const = 0;

	virtual void activateAllValves() = 0;
	virtual void desactivateAllValves() = 0;
};

#endif // ROOT_IVANNE_H_H

// IMoteursNonAsservis.h

#ifndef IMOTEURSNONASSERVIS_H_
#define IMOTEURSNONASSERVIS_H_

class IMoteursNonAsservis {
public:
	virtual ~IMoteursNonAsservis(){};

	virtual void setPuissance(unsigned char numMoteur, char puissance) = 0;
};

#endif /*IMOTEURSNONASSERVIS_H_*/

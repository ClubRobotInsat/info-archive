//
// Created by paul on 24/04/18.
//

#ifndef ROOT_CAN_H
#define ROOT_CAN_H

#include "Callback.h"
#include "PinName.h"
#include "communication/CommunicateurPipe.h"

class CANMessage {
public:
	CANMessage() = default;
	CANMessage(int l, char* d, uint8_t i) : len(l), data(d), id(i) {}

	int len;
	char* data;
	uint8_t id;
};

class CAN : public ITrameListener {
	NamedPipe _pipe;

public:
	CAN(PinName rx, PinName tx, int baudrate = 100000) : _pipe("/tmp/write.pipe"s, "/tmp/read.pipe"s, *this) {}

	int write(CANMessage message) {
		for(int i = 0; i < message.len; ++i) {
			_pipe.envoyer(message.data[i]);
		}
		return message.len;
	}

	int read(CANMessage& msg, int handle = 0) {
		uint8_t id_cmd, idFort, nbDonnees, donnees[255], num_paquet;

		while(!_pipe.lireOctets(id_cmd, idFort, nbDonnees, donnees, num_paquet))
			;
		msg.len = nbDonnees;
		msg.id = idFort;
		strncpy(msg.data, (char*)&donnees[0], nbDonnees);

		return msg.len;
	}

	void reset();

	enum IrqType {
		RxIrq = 0,
		TxIrq,
		EwIrq,
		DoIrq,
		WuIrq,
		EpIrq,
		AlIrq,
		BeIrq,
		IdIrq,

		IrqCnt
	};

	void attach(Callback<void()> func, IrqType type = RxIrq) {}

	bool onRecu(Trame const& trame, bool isAck) override {
		return false;
	}
};


#endif // ROOT_CAN_H

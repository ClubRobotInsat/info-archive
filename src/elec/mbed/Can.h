//
// Created by paul on 24/04/18.
//

#ifndef ROOT_CAN_H
#define ROOT_CAN_H


#include "Callback.h"
#include "PinName.h"

class CANMessage {
public:
	CANMessage() = default;
	CANMessage(int l, char* d, uint8_t i) : len(l), data(d), id(i) {}

	int len;
	char* data;
	uint8_t id;
};

class CAN {

public:
	CAN(PinName rx, PinName tx, int baudrate = 100000){};
	int write(CANMessage message) {
		return 0;
	}
	int read(CANMessage& msg, int handle = 0) {
		return 0;
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
};


#endif // ROOT_CAN_H

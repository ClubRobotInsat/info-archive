//
// Created by paul on 24/04/18.
//

#ifndef ROOT_SERIAL_H
#define ROOT_SERIAL_H


#include "Callback.h"
#include "PinName.h"
#include <cstdint>
#include <vector>

enum Event {
	SERIAL_EVENT_TX_ALL,
	SERIAL_EVENT_RX_ALL,
};

enum DMAUsage {
	DMA_USAGE_OPPORTUNISTIC,
	DMA_USAGE_NEVER,
	DMA_USAGE_ALWAYS,
};

class Serial {
public:
	Serial(PinName rx, PinName tx, int baudrate = 100000) : _rx{}, _tx{}, _rx_pin(rx), _tx_pin(tx) {}

	void write(uint8_t* data, uint8_t size, event_callback_t e, Event event){};

	void read(uint8_t* buffer, uint8_t size, event_callback_t c, Event e){};

	void set_dma_usage_tx(DMAUsage usage){};
	void set_dma_usage_rx(DMAUsage usage){};

private:
	std::vector<uint8_t> _rx;
	std::vector<uint8_t> _tx;
	PinName _rx_pin;
	PinName _tx_pin;
};


#endif // ROOT_SERIAL_H

//
// Created by paul on 24/04/18.
//

#include "Serial.h"
#include "Interface.h"

void Serial::write(uint8_t* data, uint8_t size, event_callback_t e, Event event) {
	for(int i = 0; i < size; i++) {
		interface_elec_info.elec_put_data(data[i]);
	}
	e.call(event);
}

void Serial::read(uint8_t* buffer, uint8_t size, event_callback_t c, Event e) {
	while(interface_elec_info.queue_size_info() < size) {
	}

	for(int i = 0; i < size; i++) {
		buffer[i] = interface_elec_info.elec_get_data();
	}

	c.call(e);
}

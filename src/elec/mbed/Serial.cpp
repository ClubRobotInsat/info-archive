//
// Created by paul on 24/04/18.
//

#include "Serial.h"

void Serial::write(uint8_t* data, uint8_t size, event_callback_t e, Event event) {
	e.call(event);
}

void Serial::read(uint8_t* buffer, uint8_t size, event_callback_t c, Event e) {
	c.call(e);
}

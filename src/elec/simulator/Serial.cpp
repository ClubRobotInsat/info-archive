//
// Created by paul on 24/04/18.
//

#include "Serial.h"
#include "Interface.h"
#include <iostream>
#include <thread>

void write_threaded(uint8_t* data, uint8_t size, event_callback_t e, Event event) {
	Serial::_writing.lock();
	{
		std::lock_guard<std::mutex> lock(Interface::_mutex_elec);
		for(int i = 0; i < size; i++) {
			interface_elec_info.elec_put_data(data[i]);
		}
		std::this_thread::sleep_for(std::chrono::microseconds(5));
	}
	e.call(event);
	Serial::_writing.unlock();
}

void read_threaded(uint8_t* buffer, uint8_t size, event_callback_t c, Event e) {
	Serial::_reading.lock();
	while(interface_elec_info.queue_size_info() < size) {
		std::this_thread::sleep_for(std::chrono::microseconds(5));
	}
	{
		std::lock_guard<std::mutex> lock(Interface::_mutex_info);
		for(int i = 0; i < size; i++) {
			buffer[i] = interface_elec_info.elec_get_data();
		}
	}
	c.call(e);
	Serial::_reading.unlock();
}

void Serial::write(uint8_t* data, uint8_t size, event_callback_t e, Event event) {
	_write = std::thread(write_threaded, data, size, e, event);
	_write.detach();
}

void Serial::read(uint8_t* buffer, uint8_t size, event_callback_t c, Event e) {
	_read = std::thread(read_threaded, buffer, size, c, e);
	_read.detach();
}

std::mutex Serial::_writing;
std::mutex Serial::_reading;
//
// Created by paul on 24/04/18.
//

#ifndef ROOT_INTERFACE_H_H
#define ROOT_INTERFACE_H_H

#include <cstdint>
#include <mutex>
#include <queue>

class Interface {
public:
	Interface() {}

	uint8_t info_get_data();

	uint8_t elec_get_data();

	void info_put_data(uint8_t data);

	void elec_put_data(uint8_t data);

	long queue_size_elec();

	long queue_size_info();

	static std::mutex _mutex_info;
	static std::mutex _mutex_elec;

private:
	static std::queue<uint8_t> _from_info;
	static std::queue<uint8_t> _from_elec;
};

static Interface interface_elec_info;

#endif // ROOT_INTERFACE_H_H

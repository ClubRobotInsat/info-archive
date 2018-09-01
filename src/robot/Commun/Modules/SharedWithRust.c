//
// Created by terae on 8/25/18.
//

#include "SharedWithRust.h"
#include <stddef.h>

#define FRAME_SERVO_SIZE(number) (1 + (number)*7)

SharedServos2019 servo_read_frame(const uint8_t* message, uint8_t size) {
	SharedServos2019 s;

	if(message == NULL || size == 0) {
		s.parsing_failed = 1;
		return s;
	}
	uint8_t count = 0;
	uint8_t servo_nb = message[count++];

	if(size != FRAME_SERVO_SIZE(servo_nb)) {
		s.parsing_failed = 1;
		return s;
	}

	for(uint8_t i = 0; i < 8; ++i) {
		s.servos[i].id = 0;
	}

	for(int i = 0; i < servo_nb; ++i) {
		uint8_t id = message[count++];

		if(id == 0 || id >= 8) {
			s.parsing_failed = 1;
			return s;
		}

		s.servos[id].id = id;

		uint16_t position = message[count++];
		position <<= 8;
		position |= message[count++];
		s.servos[id].position = position;

		uint16_t wanted_position = message[count++];
		wanted_position <<= 8;
		wanted_position |= message[count++];
		s.servos[id].wanted_position = wanted_position;

		s.servos[id].speed = message[count++];

		uint8_t infos = message[count++];
		s.servos[id].blocked = (char)((0b00010000 & infos) >> 4);
		s.servos[id].blocking_mode = (uint8_t)((0b00001000 & infos) >> 3);
		s.servos[id].color = (uint8_t)(0b00000111 & infos);
	}

	s.parsing_failed = 0;
	return s;
}

uint8_t servo_write_frame(uint8_t* buf, uint8_t buf_size, const SharedServos2019* obj) {
	uint8_t size = 0;
	uint8_t servo_nb = 0;

	if(buf == NULL || obj == NULL || buf_size == 0) {
		return 0;
	}

	for(uint8_t id = 0; id < 8; ++id) {
		if(obj->servos[id].id > 0) {
			++servo_nb;
		}
	}

	// Il n'y a pas assez de place dans le buffer : on n'écrit rien dedans
	if(buf_size < FRAME_SERVO_SIZE(servo_nb)) {
		return 0;
	}

	buf[size++] = servo_nb;
	for(uint8_t id = 0; id < 8; ++id) {
		if(obj->servos[id].id > 0) {
			buf[size++] = obj->servos[id].id;

			buf[size++] = (uint8_t)((UINT8_MAX - ((0xff00 & obj->servos[id].position) >> 8)) ^ UINT8_MAX);
			buf[size++] = (uint8_t)((UINT8_MAX - obj->servos[id].position) ^ UINT8_MAX);

			buf[size++] = (uint8_t)((UINT8_MAX - ((0xff00 & obj->servos[id].wanted_position) >> 8)) ^ UINT8_MAX);
			buf[size++] = (uint8_t)((UINT8_MAX - obj->servos[id].wanted_position) ^ UINT8_MAX);

			buf[size++] = obj->servos[id].speed;

			uint8_t infos = (uint8_t)obj->servos[id].blocked;
			infos <<= 1;
			infos |= obj->servos[id].blocking_mode;
			infos <<= 3;
			infos |= obj->servos[id].color;
			buf[size++] = infos;
		}
	}

	return size;
}

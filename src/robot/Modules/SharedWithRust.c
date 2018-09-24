//
// Created by terae on 8/25/18.
//

#include "SharedWithRust.h"
#include <stddef.h>

#define FRAME_SERVO_SIZE(number) (1 + (number)*7)
#define FRAME_MOTOR_SIZE(controlled, uncontrolled, brushless) (3 + (controlled)*4 + (uncontrolled)*2 + (brushless)*2)

SharedServos2019 servo_read_frame(const uint8_t* message, uint8_t size) {
	SharedServos2019 s;

	if(message == NULL || size == 0) {
		s.parsing_failed = 1;
		return s;
	}
	uint8_t count = 0;
	uint8_t nb_servo = message[count++];

	if(size != FRAME_SERVO_SIZE(nb_servo)) {
		s.parsing_failed = 1;
		return s;
	}

	for(uint8_t i = 0; i < MAX_SERVOS; ++i) {
		s.servos[i].id = 0;
	}

	for(int i = 0; i < nb_servo; ++i) {
		uint8_t id = message[count++];

		if(id == 0 || id >= MAX_SERVOS) {
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
	uint8_t nb_servo = 0;

	if(buf == NULL || obj == NULL || buf_size == 0) {
		return 0;
	}

	for(uint8_t id = 0; id < MAX_SERVOS; ++id) {
		if(obj->servos[id].id > 0) {
			++nb_servo;
		}
	}

	// Il n'y a pas assez de place dans le buffer : on n'écrit rien dedans
	if(buf_size < FRAME_SERVO_SIZE(nb_servo)) {
		return 0;
	}

	buf[size++] = nb_servo;
	for(uint8_t id = 0; id < MAX_SERVOS; ++id) {
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SharedMotors2019 motor_read_frame(const uint8_t* message, uint8_t size) {
	SharedMotors2019 s;

	if(message == NULL || size < 3) {
		s.parsing_failed = 1;
		return s;
	}
	uint8_t count = 0;
	uint8_t nb_controlled = message[count++];
	uint8_t nb_uncontrolled = message[count++];
	uint8_t nb_brushless = message[count++];

	if(size != FRAME_MOTOR_SIZE(nb_controlled, nb_uncontrolled, nb_brushless)) {
		s.parsing_failed = 1;
		return s;
	}

	for(uint8_t i = 0; i < MAX_CONTROLLED_MOTORS; ++i) {
		s.controlled_motors[i].id = 0;
	}

	for(uint8_t i = 0; i < MAX_UNCONTROLLED_MOTORS; ++i) {
		s.uncontrolled_motors[i].id = 0;
	}

	for(uint8_t i = 0; i < MAX_BRUSHLESS; ++i) {
		s.brushless[i].id = 0;
	}

	for(int i = 0; i < nb_controlled; ++i) {
		uint8_t id = message[count++];

		if(id == 0 || id >= MAX_CONTROLLED_MOTORS) {
			s.parsing_failed = 1;
			return s;
		}

		s.controlled_motors[id].id = id;
		s.controlled_motors[id].wanted_angle_position = message[count++];
		s.controlled_motors[id].wanted_nb_turns = message[count++];

		uint8_t infos = message[count++];
		s.controlled_motors[id].finished = (uint8_t)((0b00000010 & infos) >> 1);
		s.controlled_motors[id].new_command = (uint8_t)(0b00000001 & infos);
	}

	for(int i = 0; i < nb_uncontrolled; ++i) {
		uint8_t id = message[count++];

		if(id == 0 || id >= MAX_CONTROLLED_MOTORS) {
			s.parsing_failed = 1;
			return s;
		}

		s.uncontrolled_motors[id].id = id;
		uint8_t infos = message[count++];
		s.uncontrolled_motors[id].on_off = (uint8_t)((0b00000010 & infos) >> 1);
		s.uncontrolled_motors[id].rotation = (uint8_t)(0b00000001 & infos);
	}

	for(int i = 0; i < nb_brushless; ++i) {
		uint8_t id = message[count++];

		if(id == 0 || id >= MAX_CONTROLLED_MOTORS) {
			s.parsing_failed = 1;
			return s;
		}

		s.brushless[id].id = id;
		s.brushless[id].on_off = message[count++];
	}

	s.parsing_failed = 0;
	return s;
}

uint8_t motor_write_frame(uint8_t* buf, uint8_t buf_size, const SharedMotors2019* obj) {
	uint8_t size = 0;
	uint8_t nb_controlled = 0;
	uint8_t nb_uncontrolled = 0;
	uint8_t nb_brushless = 0;

	if(buf == NULL || obj == NULL || buf_size == 0) {
		return 0;
	}

	// Compte de chaque type de moteurs
	for(uint8_t id = 0; id < MAX_CONTROLLED_MOTORS; ++id) {
		if(obj->controlled_motors[id].id > 0) {
			++nb_controlled;
		}
	}

	for(uint8_t id = 0; id < MAX_UNCONTROLLED_MOTORS; ++id) {
		if(obj->uncontrolled_motors[id].id > 0) {
			++nb_uncontrolled;
		}
	}

	for(uint8_t id = 0; id < MAX_BRUSHLESS; ++id) {
		if(obj->brushless[id].id > 0) {
			++nb_brushless;
		}
	}

	// Il n'y a pas assez de place dans le buffer : on n'écrit rien dedans
	if(buf_size < FRAME_MOTOR_SIZE(nb_controlled, nb_uncontrolled, nb_brushless)) {
		return 0;
	}

	// Ecriture de chaque type de moteur
	buf[size++] = nb_controlled;
	buf[size++] = nb_uncontrolled;
	buf[size++] = nb_brushless;

	for(uint8_t id = 0; id < MAX_CONTROLLED_MOTORS; ++id) {
		if(obj->controlled_motors[id].id > 0) {
			buf[size++] = obj->controlled_motors[id].id;

			buf[size++] = obj->controlled_motors[id].wanted_angle_position;
			buf[size++] = obj->controlled_motors[id].wanted_nb_turns;

			uint8_t infos = obj->controlled_motors[id].finished;
			infos <<= 1;
			infos |= obj->controlled_motors[id].new_command;
			buf[size++] = infos;
		}
	}

	for(uint8_t id = 0; id < MAX_UNCONTROLLED_MOTORS; ++id) {
		if(obj->uncontrolled_motors[id].id > 0) {
			buf[size++] = obj->uncontrolled_motors[id].id;

			uint8_t infos = obj->uncontrolled_motors[id].on_off;
			infos <<= 1;
			infos |= obj->uncontrolled_motors[id].rotation;
			buf[size++] = infos;
		}
	}

	for(uint8_t id = 0; id < MAX_BRUSHLESS; ++id) {
		if(obj->brushless[id].id > 0) {
			buf[size++] = obj->brushless[id].id;

			buf[size++] = obj->brushless[id].on_off;
		}
	}

	return size;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SharedAvoidance2019 avoidance_read_frame(const uint8_t* message, uint8_t size) {
	SharedAvoidance2019 s;
	s.parsing_failed = 1;
	return s;
}

uint8_t avoidance_write_frame(uint8_t* buf, uint8_t buf_size, const SharedAvoidance2019* obj) {
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SharedIO2019 io_read_frame(const uint8_t* message, uint8_t size) {
	SharedIO2019 s;

	if(message == NULL || size == 0) {
		s.parsing_failed = 1;
		return s;
	}

	s.tirette = message[0];
	s.parsing_failed = 0;

	return s;
}

uint8_t io_write_frame(uint8_t* buf, uint8_t buf_size, const SharedIO2019* obj) {
	if(buf == NULL || obj == NULL || buf_size == 0) {
		return 0;
	}

	buf[0] = obj->tirette;
	return 1;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

SharedMoving2019 moving_read_frame(const uint8_t* message, uint8_t size) {
	SharedMoving2019 s;
	s.parsing_failed = 1;
	return s;
}

uint8_t moving_write_frame(uint8_t* buf, uint8_t buf_size, const SharedMoving2019* obj) {
	return 0;
}

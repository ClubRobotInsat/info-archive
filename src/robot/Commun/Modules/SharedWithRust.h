//
// Created by terae on 8/25/18.
//

// Ce fichier regroupe des structures codées en C qui sont partagées avec l'électronique Rust
#ifndef ROOT_SHAREDWITHRUST_H
#define ROOT_SHAREDWITHRUST_H

#include <stdint.h>

#define MAX_SERVOS 8

// Allows the header to be use from both C (so Rust) and C++
#ifdef __cplusplus
extern "C" {
#endif

/// Structure commune de l'ensemble des servos
typedef struct SharedServos2019 {
	/// Composition interne d'un servo-moteur
	struct Servo2019 {
		uint8_t id; // 'id == 0' veut dire qu'il n'y a pas de servo-moteur
		uint16_t position;
		uint16_t wanted_position;
		uint8_t speed;
		char blocked;
		uint8_t blocking_mode;
		uint8_t color;
	} servos[MAX_SERVOS];

	uint8_t parsing_failed;

} SharedServos2019;

/// Fonctions définies en C et utilisées à la fois dans le code C++ et Rust

// Format d'une trame :
// <nb_servo: u8>
// <[<id: u8> <position: u16> <wanted_position: u16> <speed: u8> <blocking data, color: u8>] ...>
extern SharedServos2019 servo_read_frame(const uint8_t* message, uint8_t size);
extern uint8_t servo_write_frame(uint8_t* buf, uint8_t buf_size, const SharedServos2019* obj);

#ifdef __cplusplus
}
#endif

#endif // ROOT_SHAREDWITHRUST_H

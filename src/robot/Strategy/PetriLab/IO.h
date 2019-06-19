//
// Created by terae on 25/03/19.
//

#pragma once

#include "../ModuleInterfacers/IOInterfacer.h"

// @PetriLab@ IO

using BuzzerState = Strategy::Interfacer::IOInterfacer::BuzzerState;

Outcome play_success_sound();
Outcome play_error_sound();

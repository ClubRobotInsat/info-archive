//
// Created by terae on 25/03/19.
//

#pragma once

#include "../ModuleInterfacers/IOInterfacer.h"

// @PetriLab@ IO

using BuzzerState = Strategy::Interfacer::IOInterfacer::BuzzerState;

ActionResult play_success_sound();
ActionResult play_error_sound();

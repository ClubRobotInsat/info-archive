//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacerSecondary.h"
#include "Utils.h"
#include <Outcome.h>

// @PetriLab@ Pumps

using AtomType = Strategy::AtomType;

Outcome catch_atom();
AtomType get_color_atom();
Outcome release_atom();

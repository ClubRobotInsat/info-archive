//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacerPrimary.h"
#include "Utils.h"
#include <Outcome.h>

// @PetriLab@ Pumps

using AtomType = Strategy::AtomType;

Outcome catch_front(AtomType left, AtomType right);
Outcome catch_back(AtomType left, AtomType right);

Outcome release_all();

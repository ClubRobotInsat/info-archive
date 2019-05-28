//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacerPrimary.h"
#include "Utils.h"
#include <ActionResult.h>

// @PetriLab@ Pumps

using AtomType = Strategy::AtomType;

ActionResult catch_front(AtomType left, AtomType right);
ActionResult catch_back(AtomType left, AtomType right);

ActionResult release_all();

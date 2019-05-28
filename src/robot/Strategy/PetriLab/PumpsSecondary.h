//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacerSecondary.h"
#include "Utils.h"
#include <ActionResult.h>

// @PetriLab@ Pumps

using AtomType = Strategy::AtomType;

ActionResult catch_atom(AtomType atom);
ActionResult release_atom();

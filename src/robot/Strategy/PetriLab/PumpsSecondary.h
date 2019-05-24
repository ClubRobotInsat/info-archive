//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacer.h"
#include "Utils.h"
#include <ActionResult.h>

// @PetriLab@ Pumps

using AtomType = Strategy::Interfacer::PumpsInterfacer::AtomType;

ActionResult catch_atom(AtomType atom);
ActionResult release_atom();

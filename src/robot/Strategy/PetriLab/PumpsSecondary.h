//
// Created by terae on 24/03/19.
//

#pragma once

#include "../ModuleInterfacers/PumpsInterfacer.h"
#include "Utils.h"
#include <ActionResult.h>

// @PetriLab@ Pumps

using StockingRail = Strategy::Interfacer::PumpsInterfacer::StockingRail;

ActionResult catch_forward(StockingRail left, StockingRail right);
ActionResult catch_backward(StockingRail left, StockingRail right);

ActionResult release_forward();
ActionResult release_backward();

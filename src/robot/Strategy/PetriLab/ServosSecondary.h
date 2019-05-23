//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <ActionResult.h>

// @PetriLab@ ServosSecondary

ActionResult raise_arm(Angle);
ActionResult rotate_arm(Angle);
ActionResult elevator(Angle);

ActionResult arm_ground();
ActionResult arm_top_right();

ActionResult raise_elevator();
ActionResult lower_elevator();

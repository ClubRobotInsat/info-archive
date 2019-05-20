//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <ActionResult.h>

ENUM_CLASS(ArmPosition, BOTTOM_HORIZONTAL, BOTTOM_VERTICAL, BOTTOM_GOLDENIUM, TOP_EXTERNAL_RAIL, TOP_INTERNAL_RAIL);

// @PetriLab@ Servos

ActionResult forward_arm(Angle);
ActionResult forward_hand(Angle);
ActionResult backward_arm(Angle);
ActionResult backward_hand(Angle);

ActionResult forward_bottom_horizontal();
ActionResult forward_bottom_vertical();
ActionResult forward_bottom_goldenium();
ActionResult forward_top_external_rail();
ActionResult forward_top_center_rail();
ArmPosition get_forward_position();

ActionResult backward_bottom_horizontal();
ActionResult backward_bottom_vertical();
ActionResult backward_bottom_goldenium();
ActionResult backward_top_external_rail();
ActionResult backward_top_center_rail();
ArmPosition get_backward_position();

//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <ActionResult.h>

ENUM_CLASS(Arm, FRONT, BACK)
ENUM_CLASS(ArmPosition, BOTTOM_HORIZONTAL, BOTTOM_VERTICAL, TOP_VERTICAL, CATCH_GOLDENIUM, RELEASE_GOLDENIUM, TOP_EXTERNAL_STORAGE, TOP_INTERNAL_STORAGE)
ENUM_CLASS(ExternalStorage, FRONT_LEFT, FRONT_RIGHT, BACK_LEFT, BACK_RIGHT)

// @PetriLab@ Servos

ActionResult front_arm(Angle);
ActionResult front_hand(Angle);
ActionResult back_arm(Angle);
ActionResult back_hand(Angle);
ActionResult arm_position(Arm arm, Angle angle);
ActionResult hand(Arm arm, Angle angle);

ActionResult external_storage(ExternalStorage, Angle);
ActionResult internal_storage(Angle);

ArmPosition get_front_position();
ArmPosition get_back_position();

ActionResult arm_bottom_horizontal(Arm arm);
ActionResult arm_bottom_vertical(Arm arm);
ActionResult arm_top_vertical(Arm arm);
ActionResult arm_catch_goldenium(Arm arm);
ActionResult arm_release_goldenium(Arm arm);
ActionResult arm_external_storage(Arm arm);
ActionResult arm_internal_storage(Arm arm);

ActionResult open_external_storage(ExternalStorage);
ActionResult close_external_storage(ExternalStorage);
ActionResult open_internal_storage();
ActionResult close_internal_storage();

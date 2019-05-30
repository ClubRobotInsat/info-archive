//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <ActionResult.h>

ENUM_CLASS(Arm, Front, Back)
ENUM_CLASS(ArmPosition, BottomHorizontal, BottomVertical, TopVertical, CatchGoldenium, ReleaseGoldenium, TopExternalStorage, TopInternalStorage, Unknown)
ENUM_CLASS(ExternalStorage, FrontLeft, FrontRight, BackLeft, BackRight)

// @PetriLab@ ServosHelper

ActionResult front_arm(Angle);
ActionResult front_hand(Angle);
ActionResult back_arm(Angle);
ActionResult back_hand(Angle);
ActionResult arm_position(Arm arm, Angle angle);
ActionResult hand_position(Arm arm, Angle angle);

ArmPosition get_front_position();
ArmPosition get_back_position();

double get_angle_by_id(int id);
ActionResult set_angle_by_id(Angle angle, int id);

// @PetriLab@ Servos

ActionResult arm_bottom_horizontal(Arm arm);
ActionResult arm_bottom_vertical(Arm arm);
ActionResult arm_top_vertical(Arm arm);
ActionResult arm_catch_goldenium(Arm arm);
ActionResult arm_release_goldenium(Arm arm);
ActionResult arm_external_storage(Arm arm);
// ActionResult arm_internal_storage(Arm arm);

ActionResult open_external_storage(ExternalStorage);
ActionResult close_external_storage(ExternalStorage);
// ActionResult open_internal_storage();
// ActionResult close_internal_storage();

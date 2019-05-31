//
// Created by terae on 01/02/19.
//

#pragma once

#include "Utils.h"
#include <Outcome.h>

ENUM_CLASS(Arm, Front, Back)
ENUM_CLASS(ArmPosition, BottomHorizontal, BottomVertical, TopVertical, CatchGoldenium, ReleaseGoldenium, TopExternalStorage, TopInternalStorage, Unknown)
ENUM_CLASS(ExternalStorage, FrontLeft, FrontRight, BackLeft, BackRight)

// @PetriLab@ ServosHelper

Outcome front_arm(Angle);
Outcome front_hand(Angle);
Outcome back_arm(Angle);
Outcome back_hand(Angle);
Outcome arm_position(Arm arm, Angle angle);
Outcome hand_position(Arm arm, Angle angle);

ArmPosition get_front_position();
ArmPosition get_back_position();

// Retourne la place occupée par les bras devant le robot pour mettre à jour la distance d'évitement
// FIXME : ce code est dégueulasse, il faut le supprimer après la Coupe (le design est fucked up)
Distance get_actuators_offset();

double get_angle_by_id(int id);
Outcome set_angle_by_id(Angle angle, int id);

// @PetriLab@ Servos

Outcome arm_bottom_horizontal(Arm arm);
Outcome arm_bottom_vertical(Arm arm);
Outcome arm_top_vertical(Arm arm);
Outcome arm_catch_goldenium(Arm arm);
Outcome arm_release_goldenium(Arm arm);
Outcome arm_external_storage(Arm arm);
Outcome arm_activate_experience(Arm arm);
// Outcome arm_internal_storage(Arm arm);

Outcome open_external_storage(ExternalStorage);
Outcome close_external_storage(ExternalStorage);
// Outcome open_internal_storage();
// Outcome close_internal_storage();

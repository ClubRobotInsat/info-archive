//
// Created by terae on 01/02/19.
//

#pragma once

#include <ActionResult.h>
#include <Constants.h>

#include "../ModuleInterfacers/NavigationInterfacer.h"

using SensAdvance = Strategy::Interfacer::NavigationInterfacer::SensAdvance;
using SensRotation = Strategy::Interfacer::NavigationInterfacer::SensRotation;

// @PetriLab@ Navigation

/// Linear moves; stop if detect an adversary
ActionResult forward(Distance, SensAdvance, Duration timeout);
ActionResult forward(Distance, SensAdvance);
ActionResult forward(Distance, Duration timeout);
ActionResult backward(Distance);

/// Lienar moves; moves until a blocking element
ActionResult forward_infinity(Duration timeout = GLOBAL_CONSTANTS().get_match_duration());
ActionResult backward_infinity(Duration timeout = GLOBAL_CONSTANTS().get_match_duration());

/// Stop
ActionResult stop();
ActionResult emergency_stop();

/// Angular moves
ActionResult turn_relative(Angle);
ActionResult turn_absolute(Angle, SensRotation);

/// A* moves; all is automatized
ActionResult move_to(Distance x, Distance y, Angle final_angle, SensAdvance);
ActionResult move_to(Distance x, Distance y, Angle final_angle, SensAdvance, Duration timeout);
ActionResult move_to_vec(Vector2m, Angle final_angle);
ActionResult move_to_vec(Vector2m, Angle final_angle, SensAdvance, Duration timeout = 25_s);

// @PetriLab@

/// Functions used often
ActionResult forward(Distance);
ActionResult turn_absolute(Angle);
ActionResult move_to(Distance x, Distance y, Angle final_angle);

// @PetriLab@ NavigationHelpers

Vector2m get_position();
ActionResult set_position(Distance x, Distance y);

Angle get_angle();
ActionResult set_angle(Angle angle);

ActionResult set_coordinates(Distance x, Distance y, Angle angle);

// Returns the distance as the crow flies between the robot and a given position
Distance get_distance_robot_position(Distance x, Distance y);

ActionResult recaling_top(SensAdvance sens, Distance y);
ActionResult recaling_bottom(SensAdvance sens, Distance y);
ActionResult recaling_right(SensAdvance sens, Distance x);
ActionResult recaling_left(SensAdvance sens, Distance x);

ActionResult activate_asserv();
ActionResult deactivate_asserv();

// @PetriLab@ Speed

ActionResult set_fast_linear_speed();
ActionResult set_slow_linear_speed();
ActionResult set_linear_speed(Speed);
ActionResult reset_linear_speed();
Speed get_linear_speed();

ActionResult set_fast_angular_speed();
ActionResult set_slow_angular_speed();
ActionResult set_angular_speed(AngularSpeed);
ActionResult reset_angular_speed();
AngularSpeed get_angular_speed();

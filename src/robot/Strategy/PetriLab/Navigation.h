//
// Created by terae on 01/02/19.
//

#pragma once

#include <Constants.h>
#include <Outcome.h>

#include "../ModuleInterfacers/NavigationInterfacer.h"

using SensAdvance = Strategy::Interfacer::NavigationInterfacer::SensAdvance;
using SensRotation = Strategy::Interfacer::NavigationInterfacer::SensRotation;

// @PetriLab@ Navigation

/// Linear moves; stop if detect an adversary
Outcome forward(Distance, SensAdvance, Duration timeout);
Outcome forward(Distance, SensAdvance);
Outcome forward(Distance, Duration timeout);
Outcome backward(Distance);

/// Lienar moves; moves until a blocking element
Outcome forward_infinity(Duration timeout = GLOBAL_CONSTANTS().get_match_duration());
Outcome backward_infinity(Duration timeout = GLOBAL_CONSTANTS().get_match_duration());

/// Stop
Outcome stop();
Outcome emergency_stop();

/// Angular moves
Outcome turn_relative(Angle);
Outcome turn_absolute(Angle, SensRotation);

/// A* moves; all is automatized
Outcome move_to(Distance x, Distance y, Angle final_angle, SensAdvance);
Outcome move_to(Distance x, Distance y, Angle final_angle, SensAdvance, Duration timeout);
Outcome move_to_vec(Vector2m, Angle final_angle);
Outcome move_to_vec(Vector2m, Angle final_angle, SensAdvance, Duration timeout = 25_s);

// @PetriLab@

/// Functions used often
Outcome forward(Distance);
Outcome turn_absolute(Angle);
Outcome move_to(Distance x, Distance y, Angle final_angle);

// @PetriLab@ NavigationHelpers

Vector2m get_position();
Outcome set_position(Distance x, Distance y);

Angle get_angle();
Outcome set_angle(Angle angle);

Outcome set_coordinates(Distance x, Distance y, Angle angle);

// Returns the distance as the crow flies between the robot and a given position
Distance get_distance_robot_position(Distance x, Distance y);

Outcome recaling_top(SensAdvance sens);
Outcome recaling_bottom(SensAdvance sens);
Outcome recaling_right(SensAdvance sens);
Outcome recaling_left(SensAdvance sens);

Outcome recaling_top(SensAdvance sens, Distance offset_y);
Outcome recaling_bottom(SensAdvance sens, Distance offset_y);
Outcome recaling_right(SensAdvance sens, Distance offset_x);
Outcome recaling_left(SensAdvance sens, Distance offset_x);

Outcome activate_asserv();
Outcome deactivate_asserv();

// @PetriLab@ Speed

Outcome set_fast_linear_speed();
Outcome set_slow_linear_speed();
Outcome set_linear_speed(Speed);
Outcome reset_linear_speed();
Speed get_linear_speed();

Outcome set_fast_angular_speed();
Outcome set_slow_angular_speed();
Outcome set_angular_speed(AngularSpeed);
Outcome reset_angular_speed();
AngularSpeed get_angular_speed();

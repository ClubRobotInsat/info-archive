//
//  ConstantesPrincipal.cpp
//  Club Robot
//
//  Created by Rémi on 28/10/.
//

#include "Constantes.h"

namespace ConstantesPrincipal {
	// Angle const positionPince[enumToInt(PositionPince::NBR)] = {50_deg, 67.5_deg}; // TODO: Modifier les valeurs pour cette année.

	Distance const ENTRAXE_PINCES = 12_cm;
	Distance const DISTANCE_CENTRE_PINCES = 30_cm;
}

Duration const ConstantesPrincipal::TIMEOUT_MOVE_DEFAULT = Constantes::TIMEOUT_MOVE_DEFAULT;
Duration const ConstantesPrincipal::TIMEOUT_OPPONENT_BLOCKING_DEFAULT = Constantes::TIMEOUT_OPPONENT_BLOCKING_DEFAULT;
Speed const ConstantesPrincipal::LINEAR_SPEED_DEFAULT = Constantes::LINEAR_SPEED_DEFAULT;
AngularSpeed const ConstantesPrincipal::ANGULAR_SPEED_DEFAULT = 3.75_rad_s;
Distance const ConstantesPrincipal::LINEAR_PRECISION_DEFAULT = Constantes::LINEAR_PRECISION_DEFAULT;
Angle const ConstantesPrincipal::ANGULAR_PRECISION_DEFAULT = Constantes::ANGULAR_PRECISION_DEFAULT;

Distance const ConstantesPrincipal::ROBOT_RAY_ROTATION = 230.0_mm;
Vector3m const ConstantesPrincipal::ROBOT_SIZE = {30_cm, 30_cm, 40_cm};
uint16_t const ConstantesPrincipal::TCPIP_PORT_SIMU_DEFAULT = 4321;
Angle const ConstantesPrincipal::OFFSET_ANGLE_ADV = -0.5_PI;

int const ConstantesPrincipal::IDRobot = 1;

Duration ConstantesRobotPrincipal::getTimeoutMoveDefault() const {
	return ConstantesPrincipal::TIMEOUT_MOVE_DEFAULT;
}

Duration ConstantesRobotPrincipal::getTimeoutOpponentBlockingDefault() const {
	return ConstantesPrincipal::TIMEOUT_OPPONENT_BLOCKING_DEFAULT;
}

Speed ConstantesRobotPrincipal::getLinearSpeedDefault() const {
	return ConstantesPrincipal::LINEAR_SPEED_DEFAULT;
}
AngularSpeed ConstantesRobotPrincipal::getAngularSpeedDefault() const {
	return ConstantesPrincipal::ANGULAR_SPEED_DEFAULT;
}

Distance ConstantesRobotPrincipal::getLinearPrecisionDefault() const {
	return ConstantesPrincipal::LINEAR_PRECISION_DEFAULT;
}

Angle ConstantesRobotPrincipal::getAngularPrecisionDefault() const {
	return ConstantesPrincipal::ANGULAR_PRECISION_DEFAULT;
}

Distance ConstantesRobotPrincipal::getRayRotation() const {
	return ConstantesPrincipal::ROBOT_RAY_ROTATION;
}

Vector3m ConstantesRobotPrincipal::getRobotSize() const {
	return ConstantesPrincipal::ROBOT_SIZE;
}

Angle ConstantesRobotPrincipal::getOffsetAngleAdv() const {
	return ConstantesPrincipal::OFFSET_ANGLE_ADV;
}


uint16_t ConstantesRobotPrincipal::getPortTCPIPDefault() const {
	return 1234;
}

int ConstantesRobotPrincipal::getIDRobot() const {
	return ConstantesPrincipal::IDRobot;
}

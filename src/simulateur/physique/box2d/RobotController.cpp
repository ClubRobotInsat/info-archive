
#include "RobotController.h"

#include <log/Log.h>

RobotController::RobotController(PhysicalObject& robot)
        : _robot(robot), _currentState(SimuRobotState::Idle), _blockedCounter(0) {

	reset();
}

void RobotController::reset() {
	_distanceGoal = 0_m;
	_currentDistance = 0_m;

	_angleGoal = 0_deg;
	_currentAngle = 0_deg;

	_prevPosition = _robot.getPosition();
	_prevAngle = _robot.getAngle();

	_asserv.resetPIDs();
}

void RobotController::forward(Distance distance) {
	_distanceGoal += distance;
	_currentState = SimuRobotState::Moving;
}

void RobotController::turnRelative(Angle angle) {
	_angleGoal += angle.toMinusPiPi();
	_currentState = SimuRobotState::Moving;
}

void RobotController::stop() {
	_distanceGoal = _currentDistance;
	_angleGoal = _currentAngle;
}

void RobotController::emergencyStop() {
	stop();
	_currentState = SimuRobotState::Stopped;
}

void RobotController::setCoordinates(const repere::Coordinates& coords) {
	auto currentCoords = getCoordinates();
	_positionOffset = coords.getPos2D() - currentCoords.getPos2D();
}

repere::Coordinates RobotController::getCoordinates() {
	return repere::Coordinates(_robot.getPosition() - _positionOffset, _robot.getAngle());
}

SimuRobotState RobotController::getState() {
	return _currentState;
}

void RobotController::update(Duration elapsed) {
	// Update current distance and angle
	Speed linearSpeed = _robot.getLinearVelocity();
	AngularSpeed angularSpeed = _robot.getAngularVelocity();

	Distance distanceDiff = (_robot.getPosition() - _prevPosition).norm();
	Angle angleDiff = (_robot.getAngle() - _prevAngle).toMinusPiPi();

	_currentDistance += distanceDiff * (linearSpeed > 0_m_s ? 1 : -1);
	_currentAngle += angleDiff;

	// Apply PID
	if(_currentState == SimuRobotState::Stopped) {
		_robot.setLinearVelocity(0_m_s);
		_robot.setAngularVelocity(0_rad_s);
	} else {
		_asserv.calculer(elapsed, _distanceGoal, _angleGoal, _currentDistance, _currentAngle, linearSpeed, angularSpeed);

		_robot.setLinearVelocity(linearSpeed);
		_robot.setAngularVelocity(angularSpeed);
	}

	if(_currentState == SimuRobotState::Moving) {
		// TODO use constants

		// Check if goal is reached
		if(abs(_currentDistance - _distanceGoal) < 0.4_mm && abs(_currentAngle - _angleGoal) < 0.4_deg) {
			_currentState = SimuRobotState::Idle;
		}

		// Check if the robot is blocked
		if(abs(distanceDiff) < 0.1_mm && abs(angleDiff) < 0.1_deg) {
			//_blockedCounter++;
		} else {
			_blockedCounter = 0;
		}

		if(_blockedCounter > 10) {
			_blockedCounter = 0;
			stop();
			_currentState = SimuRobotState::Blocked;
		}
	}

	// std::cout << "Linear speed: " << linearSpeed << ", Angular speed: " << angularSpeed << std::endl;
	// std::cout << "Distance goal: " << _distanceGoal << ", Current distance: " << _currentDistance << std::endl;
	// std::cout << "Angle goal: " << _angleGoal << ", Current angle: " << _currentAngle << std::endl;
	// std::cout << "State: " << _currentState << std::endl;

	// Update position data for next update
	_prevPosition = _robot.getPosition();
	_prevAngle = _robot.getAngle();
}

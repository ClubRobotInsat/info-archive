//
// Created by terae on 31/05/19.
//

#include "../AbstractStrategy.h"

#ifndef ROOT_ACTUATORSCALIBRATOR_H
#define ROOT_ACTUATORSCALIBRATOR_H

using namespace Strategy;

class ActuatorsCalibrator final : public AbstractStrategy {
public:
	explicit ActuatorsCalibrator(Constants::RobotColor color);

private:
	void funny_action() override {}

	void execute() override;
};

#endif // ROOT_ACTUATORSCALIBRATOR_H

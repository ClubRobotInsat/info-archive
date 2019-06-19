//
// Created by terae on 09/05/19.
//

#ifndef ROOT_IATEST_H
#define ROOT_IATEST_H

#include "../AbstractStrategy.h"

namespace Strategy {

	class IATest final : public AbstractStrategy {
	public:
		IATest();

	private:
		void execute() override;

		void funny_action() override;

		std::shared_ptr<PhysicalRobot::ModuleManager> _module_manager;
		std::shared_ptr<Interfacer::RobotManager> _robot_manager;

		PhysicalRobot::Servos::servo_t _id_servo;
	};

} // namespace Strategy

#endif // ROOT_IATEST_H

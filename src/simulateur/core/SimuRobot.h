#ifndef ROOT_SIMUROBOT_H
#define ROOT_SIMUROBOT_H

#include <memory>

#include "../../robot/Communication/Communicator.h"
#include "../../robot/Modules/ModuleManager.h"
#include "../communication/SimuCommunicator.h"
#include "../physique/IRobotController.h"
#include "Object3D.h"

namespace Simu {

	class SimuRobot {
	public:
		explicit SimuRobot(const std::string& name, Constants::RobotColor robotColor, Object3D& robotObject);

		void connect(std::unique_ptr<Communication::Protocol>&& protocol);

		void update(Duration time);

		JSON getFullState();

		IRobotController& getController() {
			return *_controller;
		}

		Constants::RobotColor getColor() {
			return _color;
		}

		std::string getName() {
			return _name;
		}

		PhysicalRobot::ModuleManager& getModuleManager() {
			return *_moduleMgr;
		}

	private:
		std::string _name;
		Constants::RobotColor _color;
		std::shared_ptr<IRobotController> _controller;
		Object3D& _robotObject;

		std::shared_ptr<PhysicalRobot::ModuleManager> _moduleMgr;
		std::shared_ptr<SimuCommunicator> _communicator;

		void assignModules();
	};

}; // namespace Simu

#endif // ROOT_SIMUROBOT_H

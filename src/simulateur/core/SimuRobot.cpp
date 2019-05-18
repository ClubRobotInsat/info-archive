#include "SimuRobot.h"

#include <log/Log.h>

#include "../communication/SimuLed.h"
#include "../communication/SimuNavigation.h"
#include "../communication/SimuServos.h"

namespace Simu {

	using Communication::Communicator;
	using PhysicalRobot::ModuleManager;

	SimuRobot::SimuRobot(const std::string& name, Constants::RobotColor robotColor, Object3D& robotObject)
	        : _name(name), _color(robotColor), _robotObject(robotObject), _moduleMgr(std::make_shared<ModuleManager>()) {

		_communicator = std::make_shared<SimuCommunicator>(_moduleMgr);
		_controller = std::shared_ptr<IRobotController>(robotObject.getPhysics().createRobotController());
		assignModules();
	}

	void SimuRobot::connect(std::unique_ptr<Communication::Protocol>&& protocol) {
		_communicator->connect(std::move(protocol));
	}

	void SimuRobot::update(Duration time) {
		_controller->update(time);
	}

	void SimuRobot::assignModules() {
		auto modules = GLOBAL_CONSTANTS()[_name].get_modules();

		for(auto& moduleData : modules) {
			if(moduleData.first == "navigation") {
				logDebug8("Added navigation module at id ", static_cast<int>(moduleData.second));
				_moduleMgr->add_module<SimuNavigation>(moduleData.second, _controller);
			} else if(moduleData.first == "servos") {
				logDebug8("Added servos module at id ", static_cast<int>(moduleData.second));
				_moduleMgr->add_module<SimuServos>(moduleData.second);
			}
		}
	}

} // namespace Simu

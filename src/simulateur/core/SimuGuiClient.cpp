#include "SimuGuiClient.h"

#include "../../robot/Communication/CommunicatorParsing.h"
#include "Simulateur.h"

using namespace Communication::Arguments;
using Constants::RobotColor;

SimuGuiClient::SimuGuiClient(Simulateur& simulator) : _simu(simulator) {}

SimuGuiClient::~SimuGuiClient() = default;

void SimuGuiClient::connect(const ConnectionData& connectionData) {
	std::vector<std::string> args = connectionData.parameters;
	args.insert(args.begin(), connectionData.method);

	auto result = Parser::make_protocol(args);
	if(result.first != typeid(void)) {
		logDebug("Connecting to robot with : ", connectionData.method);
		_simu._robot->connect(std::move(result.second));
	} else {
		logError("Failed to connect with given arguments");
	}
}

void SimuGuiClient::createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) {
	logDebug("Launching process : ", iaProcessData.executablePath);
}

void SimuGuiClient::testNavigationForward(Distance distance) {
	if(_simu._robot != nullptr) {
		auto& controller = _simu._robot->getController();
		controller.forward(distance);
	}
}

void SimuGuiClient::testNavigationTurn(Angle angle) {
	if(_simu._robot != nullptr) {
		auto& controller = _simu._robot->getController();
		controller.turnRelative(angle);
	}
}

void SimuGuiClient::reset(const ResetData& resetData) {
	auto color = fromString<RobotColor>(resetData.color);
	_simu.resetWorld(color);
}

std::vector<std::string> SimuGuiClient::getRobotColors() const {
	std::vector<std::string> result;

	for(RobotColor robotColor : getEnumValues<RobotColor>()) {
		result.emplace_back(toString(robotColor));
	}

	return result;
}

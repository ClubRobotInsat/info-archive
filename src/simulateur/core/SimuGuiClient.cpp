#include "SimuGuiClient.h"

#include "Simulateur.h"

SimuGuiClient::SimuGuiClient(Simulateur& simulator) : _simu(simulator) {}

SimuGuiClient::~SimuGuiClient() = default;

void SimuGuiClient::connect(const ConnectionData& connectionData) {
	logDebug("Connecting to robot with : ", connectionData.method);
	// On inverse le sens de lecture par rapport à la stratégie
	// TODO parser les arguments et se connecter comme indiqué
	std::shared_ptr<Communication::Protocol> protocol = std::make_shared<Communication::protocol_udp>("localhost", 5555, 1234);
	_simu._robot->connect(protocol);

	for(auto arg : connectionData.parameters) {
		std::cout << arg << std::endl;
	}
}

void SimuGuiClient::createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) {
	logDebug("Launching process : ", iaProcessData.executablePath);
}

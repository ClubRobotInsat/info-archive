#include "SimuGuiClient.h"

#include "Simulateur.h"

SimuGuiClient::SimuGuiClient(Simulateur& simulator) : _simu(simulator) {}

SimuGuiClient::~SimuGuiClient() = default;

void SimuGuiClient::connect(const ConnectionData& connectionData) {
	logDebug("Connecting to robot with : ", connectionData.method);

	if (connectionData.method == "UDP") {
		if (connectionData.parameters.size() < 3) {
			throw std::invalid_argument("Usage : UDP [localhost] [local port] [remote port]");
		}
		std::string host = connectionData.parameters[0];
		int localPort = std::stoi(connectionData.parameters[1]);
		int remotePort = std::stoi(connectionData.parameters[2]);

		std::shared_ptr<Communication::Protocol> protocol =
				std::make_shared<Communication::protocol_udp>(host, localPort, remotePort);
		_simu._robot->connect(protocol);
	}
}

void SimuGuiClient::createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) {
	logDebug("Launching process : ", iaProcessData.executablePath);
}

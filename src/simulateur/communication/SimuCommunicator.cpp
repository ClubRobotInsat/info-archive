#include "SimuCommunicator.h"

using Communication::Protocol;


SimuCommunicator::SimuCommunicator(const std::shared_ptr<PhysicalRobot::ModuleManager>& moduleMgr)
        : _moduleMgr(moduleMgr), _connected(false) {}

SimuCommunicator::~SimuCommunicator() {
	disconnect();
}

void SimuCommunicator::connect(std::unique_ptr<Communication::Protocol>&& protocol) {
	disconnect();

	_protocol = std::move(protocol);

	if(_protocol != nullptr) {
		_connected.store(true);
		_communicationThread = std::thread(&SimuCommunicator::communicationThread, this);
	}
}

void SimuCommunicator::disconnect() {
	if(_connected) {
		_connected.store(false);
		_communicationThread.join();
	}
}

void SimuCommunicator::communicationThread() {

	// Simulation du processus de l'électronique :
	// On reçoit la trame, on process l'état du robot et on
	// répond en renvoyant l'état du robot.
	try {
		_protocol->recv_frame(_connected, [this](const GlobalFrame& f) {
			_moduleMgr->read_frame(f);

			// récupère le nouvel état du module et l'envoie.
			std::vector<GlobalFrame> written_frames = _moduleMgr->write_frame();

			if(written_frames.empty()) {
				throw std::runtime_error("No response frame sent by the simubot module");
			} else {
				for(const GlobalFrame& frame : written_frames) {
					_protocol->send_frame(frame);
				}
			}
		});
	} catch(Protocol::ReceptionAborted& e) {
		logInfo("Stopping reception");
	}
}
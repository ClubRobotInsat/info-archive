#include "SimuCommunicator.h"

using Communication::Protocol;


SimuCommunicator::SimuCommunicator(const std::shared_ptr<PhysicalRobot::ModuleManager> &moduleMgr)
    : _moduleMgr(moduleMgr), _connected(false) {

}

SimuCommunicator::~SimuCommunicator() {
    disconnect();
}

void SimuCommunicator::connect(const std::shared_ptr<Communication::Protocol> &protocol) {
    disconnect();

    _protocol = protocol;

    if (_protocol != nullptr) {
        _connected.store(true);
        _communicationThread = std::thread(&SimuCommunicator::communicationThread, this);
    }
}

void SimuCommunicator::disconnect() {
    if (_connected) {
        _connected.store(false);
        _communicationThread.join();
    }
}

void SimuCommunicator::communicationThread() {
    // Simulation du processus de l'électronique :
    // On reçoit la trame, on process l'état du robot et on
    // répond en renvoyant l'état du robot.
    _protocol->recv_frame(_connected, [this](const GlobalFrame& f) {
        _moduleMgr->read_frame(f);

        // récupère le nouvel état du module et l'envoie.
        auto newFrame = _moduleMgr->write_frame();

        if (newFrame.has_value()) {
            _protocol->send_frame(*newFrame);
        }
        else {
            throw std::runtime_error("No response frame sent by the simubot module");
        }
    });
}
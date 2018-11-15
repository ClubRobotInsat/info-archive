#include "SimuRobot.h"

#include "../communication/SimuLed.h"
#include "../communication/SimuServos.h"

namespace Simu {

using Communication::Communicator;
using PhysicalRobot::ModuleManager;

SimuRobot::SimuRobot(const std::string &name, Object3D &robotObject) :
        _name(name),
        _robotObject(robotObject),
        _moduleMgr(std::make_shared<ModuleManager>()){

    _communicator = std::make_shared<SimuCommunicator>(_moduleMgr);
    assignModules();
}

void SimuRobot::connect(const std::shared_ptr<Communication::Protocol> &protocol) {
    _communicator->connect(protocol);
}

void SimuRobot::update() {

}

void SimuRobot::assignModules() {
    auto modules = GLOBAL_CONSTANTS[_name].get_modules();

    _moduleMgr->add_module<SimuLed>(1);
}

}

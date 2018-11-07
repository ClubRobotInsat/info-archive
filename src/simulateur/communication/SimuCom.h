#ifndef ROOT_SIMUCOM_H
#define ROOT_SIMUCOM_H

#include <memory>

#include "../../robot/Modules/ModuleManager.h"
#include "../../robot/Communication/Communicator.h"

namespace Simu {

class SimuCom {
public:
    explicit SimuCom(const std::string & connexion);

    void update();

private:
    // std::unique_ptr<Communication::Communicator> _communicator;
    std::unique_ptr<PhysicalRobot::ModuleManager> _moduleMgr;
};

};

#endif //ROOT_SIMUCOM_H

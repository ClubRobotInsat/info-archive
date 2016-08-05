//
// Created by paul on 18/07/16.
//

#ifndef ROOT_APPLICATION_H
#define ROOT_APPLICATION_H

#include <vector>
#include "../../robot/Commun/CAN.h"


class Application {
public:

    /**
     *
     * @param port The port on wich the Can object will be listening
     */
    Application(std::string const& port);

    void loop();


private:

    std::vector<int> _messageList;

    std::shared_ptr<Commun::CAN> _can;

};

#endif //ROOT_APPLICATION_H

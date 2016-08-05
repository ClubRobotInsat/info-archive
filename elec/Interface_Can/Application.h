//
// Created by paul on 18/07/16.
//

#ifndef ROOT_APPLICATION_H
#define ROOT_APPLICATION_H

#include <vector>
#include "../../robot/Commun/CAN.h"
#include "Header.h"


class Application {
public:

#ifdef ACTIVATE_CAN
    /**
     *
     * @param port The port on wich the Can object will be listening
     */
    Application(std::string const& port);
#else
    Application();
#endif
    void loop();

private:

    std::vector<int> _messageList;

    std::shared_ptr<Commun::CAN> _can;

    std::unique_ptr<GtkWidget> window;

};

#endif //ROOT_APPLICATION_H

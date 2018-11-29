#include "GtkSimuContext.h"

#include "GtkSimuApplication.h"

GtkSimuContext::GtkSimuContext(int argc, char **argv, std::string id)
        : _application(std::make_unique<GtkSimuApplication>(argc, argv, id)) {


}

void GtkSimuContext::displayMessage(const std::string &message) {

}

void GtkSimuContext::setExitHandler(const std::function<void()> &handler) {

}
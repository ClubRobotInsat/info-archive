#include "GtkSimuContext.h"

#include "GtkSimuApplication.h"

GtkSimuContext::GtkSimuContext()
        : _application(std::make_unique<GtkSimuApplication>()) {


}

void GtkSimuContext::displayMessage(const std::string &message) {

}

void GtkSimuContext::setExitHandler(const std::function<void()> &handler) {

}
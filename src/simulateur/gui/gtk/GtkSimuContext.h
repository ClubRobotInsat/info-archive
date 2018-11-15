#ifndef ROOT_GTKSIMUCONTEXT_H
#define ROOT_GTKSIMUCONTEXT_H

#include <memory>

#include "GtkInclude.h"
#include "../IGuiContext.h"

class GtkSimuApplication;

class GtkSimuContext : public IGuiContext {
public:
    GtkSimuContext();

    void displayMessage(const std::string &message) override;

    void setExitHandler(const std::function<void()> & handler) override;

private:
    std::unique_ptr<GtkSimuApplication> _application;
};


#endif //ROOT_GTKSIMUCONTEXT_H

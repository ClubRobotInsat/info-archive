#ifndef ROOT_GTKSIMUCONTEXT_H
#define ROOT_GTKSIMUCONTEXT_H

#include <memory>

#include "../IGuiContext.h"

class GtkSimuApplication;

class GtkSimuContext : public IGuiContext {
public:
    GtkSimuContext(int argc, char **argv, std::string id);

    void displayMessage(const std::string &message) override;

    void setExitHandler(const std::function<void()> & handler) override;

private:
    std::unique_ptr<GtkSimuApplication> _application;
};


#endif //ROOT_GTKSIMUCONTEXT_H

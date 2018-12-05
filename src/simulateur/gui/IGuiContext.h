#ifndef ROOT_IGUICONTEXT_H
#define ROOT_IGUICONTEXT_H

#include <functional>

class IGuiContext {
public:
	virtual ~IGuiContext() = default;

	virtual void update() = 0;

	virtual void displayMessage(const std::string& message) = 0;
};


#endif // ROOT_IGUICONTEXT_H

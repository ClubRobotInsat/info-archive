#ifndef ROOT_IGUICONTEXT_H
#define ROOT_IGUICONTEXT_H

#include <functional>

#include <Json.h>

/** Classe permettant au simulateur de controller une
 * interface utilisateur pour afficher diverses informations.
 * L'interface utilisateur peut eventuellement envoyer les
 * commandes de l'utilisateur au simulateur par le biais
 * de la classe IGuiClient. */
class IGuiContext {
public:
	virtual ~IGuiContext() = default;

	virtual void update() = 0;

	virtual void displayMessage(const std::string& message) = 0;

	virtual void displayErrorMessage(const std::string& errorMsg) = 0;

	/** Affiche l'état du robot tel que décrit dans l'objet JSON
	 * passé en paramètres. Ce JSON contient les différentes variables
	 * de chaque module. */
	virtual void displayRobotState(const JSON& robotState) = 0;

	virtual void close() = 0;
};


#endif // ROOT_IGUICONTEXT_H

#ifndef ROOT_IGUICLIENT_H
#define ROOT_IGUICLIENT_H

#include <string>
#include <vector>

#include "../physique/IRobotController.h"

struct IAProcessData {
	std::string executablePath;
	std::vector<std::string> parameters;
};

struct ConnectionData {
	std::string method;
	std::vector<std::string> parameters;
};

/** Cette classe permet à l'interface utilisateur de transmettre
 * les commandes de l'utilisateur au simulateur. */
class IGuiClient {
public:
	virtual ~IGuiClient() = default;

	/** Connexion à un programme d'IA déjà lancé. */
	virtual void connect(const ConnectionData& connectionData) = 0;

	/** Lancement d'un programme executant une IA. Une fois ce programme
	 * lancé, le simulateur s'y connecte avec les paramètres donnés. */
	virtual void createIAProcess(const IAProcessData& iaProcessData, const ConnectionData& connectionData) = 0;

	/** Donne le controller qui permet de controller le robot. Si aucun
	 * robot n'est controllable, retourne nullptr. */
	// TODO ne plus utiliser des raw pointers ?
	virtual IRobotController* getRobotController() = 0;
};

#endif // ROOT_IGUICLIENT_H

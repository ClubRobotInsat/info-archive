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

struct ResetData {
	std::string color;
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

	/** Demande au robot du simulateur d'avancer d'une certaine distance.
	 * Cette méthode est utilisée uniquement pour tester la navigation. */
	virtual void testNavigationForward(Distance distance) = 0;

	/** Demande au robot du simulateur de tourner d'un certain angle.
	 * Cette méthode est utilisée uniquement pour tester la navigation. */
	virtual void testNavigationTurn(Angle angle) = 0;

	/** Reset le simulateur, avec les données passées en paramètres. */
	virtual void reset(const ResetData& resetData) = 0;

	/** Retourne les différentes couleurs possibles de robot. */
	virtual std::vector<std::string> getRobotColors() const = 0;
};

#endif // ROOT_IGUICLIENT_H

#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../../commun/Commun.h"

#include "Modules/ModuleManager.h"

#include "Communication/ElecCommunicator.h"

namespace Commun {

	class Robot {
	public:
		/*******************************/
		/********* CONSTANTES **********/
		/*******************************/
		// TODO : déplacer ce code dans la partie 'stratégie'
		// Définit le nombre de demande d'actualisation de la carte contacteur pour être surs que la tirette est ok
		// static const int NB_RETRY_TIRETTE = 3;

		/// Initialise le robot a partir des arguments passes au programme.
		Robot(std::shared_ptr<ModuleManager> module_manager, std::vector<std::string> const& args);

		virtual ~Robot();

		template <typename Module>
		Module& get_module() const {
			return _module_manager->get_module<Module>();
		}

		// TODO : déplacer ce code dans la partie 'stratégie'
		/// Attend la tirette au départ
		// void wait_for_tirette() const;

	protected:
		std::shared_ptr<ModuleManager> _module_manager;

		/// L'initialisation des modules du robot se fait ici
		// Cette méthode n'est pas virtuelle pure
		// car on peut initialiser le module manager avant la construction du robot
		virtual void assign_modules() {}

		/// Désactivation du robot
		virtual void deactivation() {}

	private:
		std::unique_ptr<ElecCommunicator<ModuleManager>> _communicator;

		/*void setting_up_tirette() const;
		bool is_tirette_pulled() const;*/
	};
} // namespace Commun

#endif

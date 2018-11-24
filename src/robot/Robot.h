#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "Commun.h"

#include "Modules/ModuleManager.h"

#include "Communication/Communicator.h"

namespace PhysicalRobot {

	class Robot {
		/// Initialise le robot a partir des arguments passes au programme.
		/// Les modules du robot peuvent être initialisés à partir du fichier de constantes OU depuis un ModuleManager
		/// @arg name Correspond au nom de section dans le `src/robot.ini`
		Robot(std::shared_ptr<ModuleManager> module_manager, std::string name, std::vector<std::string> const& args);

	public:
		const std::string name;

		/*******************************/
		/********* CONSTANTES **********/
		/*******************************/
		// TODO : déplacer ce code dans la partie 'stratégie'
		// Définit le nombre de demande d'actualisation de la carte contacteur pour être surs que la tirette est ok
		// static const int NB_RETRY_TIRETTE = 3;

		// Initialisation du robot à partir d'un ModuleManager
		Robot(std::shared_ptr<ModuleManager> module_manager, std::vector<std::string> const& args);

		// Initialisation du robot à partir du fichier `src/robot.ini`
		Robot(std::string name, std::vector<std::string> const& args);

		virtual ~Robot();

		template <typename Module>
		Module& get_module() const {
			return _module_manager->get_module<Module>();
		}

		void set_debug(bool debug) {
			_debug_active = debug;
			if(_communicator != nullptr) {
				_communicator->set_debug(debug);
			}
		}

		// TODO : déplacer ce code dans la partie 'stratégie'
		/// Attend la tirette au départ
		// void wait_for_tirette() const;

		/// Désactivation du robot
		virtual void deactivation();

	protected:
		std::shared_ptr<ModuleManager> _module_manager;

		/// L'initialisation des modules du robot se fait ici
		// Cette méthode n'est pas virtuelle pure
		// car on peut initialiser le module manager avant la construction du robot
		virtual void assign_modules();

	private:
		std::unique_ptr<Communication::Communicator<ModuleManager>> _communicator;

		bool _debug_active;

		/*void setting_up_tirette() const;
		bool is_tirette_pulled() const;*/
	};
} // namespace PhysicalRobot

#endif

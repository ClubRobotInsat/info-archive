#ifndef _ROBOT_H_
#define _ROBOT_H_

#include "../Lidar/Driver/lidar.h"
#include "Commun.h"
#include "Communication/Communicator.h"
#include "Modules/ModuleManager.h"

namespace PhysicalRobot {

	class Robot {
		/// Initialise le robot a partir des arguments passes au programme.
		/// Les modules du robot peuvent être initialisés à partir du fichier de constantes OU depuis un ModuleManager
		/// @arg name Correspond au nom de section dans le `src/robot.ini`
		Robot(std::shared_ptr<ModuleManager> module_manager, std::string name, Lidar::LidarType, bool debug_active);

	public:
		const std::string name;

		/*******************************/
		/********* CONSTANTES **********/
		/*******************************/

		// Initialisation du robot à partir d'un ModuleManager
		Robot(std::shared_ptr<ModuleManager> module_manager, const std::vector<std::string>& args, Lidar::LidarType, bool debug_active = false);

		// Initialisation du robot à partir du fichier `src/robot.ini`
		Robot(std::string name, bool debug_active = false);
		Robot(std::string name, Lidar::LidarType, bool debug_active = false);
		// Initialisation du robot à partir du fichier `src/robot.ini` mais choix du protocol de com en ligne de cmd
		Robot(std::string name, const std::vector<std::string>& args, bool debug_active = false);
		Robot(std::string name, const std::vector<std::string>& args, Lidar::LidarType, bool debug_active = false);

		virtual ~Robot();

		template <typename Module>
		Module& get_module() const {
			return _module_manager->get_module<Module>();
		}

		template <typename Module>
		bool has_module() const {
			return _module_manager->has_module<Module>();
		}

		// map<ID, name>
		std::map<uint8_t, std::string> get_list_modules() const {
			std::map<uint8_t, std::string> result;
			for(uint8_t id : _module_manager->get_list_modules()) {
				result[id] = _module_manager->get_module_by_id(id).name;
			}
			return result;
		}

		std::type_index get_communication_protocol_type() const;

		Lidar::LidarType get_lidar_type() const;

		bool has_lidar() const;

		std::optional<FrameLidar> get_lidar_frame() const;

		void set_debug(bool debug);

		/// Désactivation du robot
		virtual void deactivation();

	protected:
		std::shared_ptr<ModuleManager> _module_manager;
		std::unique_ptr<Lidar> _lidar;

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

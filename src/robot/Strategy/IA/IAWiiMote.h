//
// Created by terae on 04/02/19.
//

#ifndef ROOT_IAWIIMOTE_H
#define ROOT_IAWIIMOTE_H

#include "../ModuleInterfacers/RobotManager.h"
#include "../wiimote/Wiimote.h"

//#define NUNCHUK_UTILISER_COURBES

namespace Strategy {
	enum TypeNavigation {
		GO_FORWARD,
		GO_BACKWARD,
		TURN_RIGHT,
		TURN_LEFT,
		DO_NOTHING,
	};

	class IAWiiMote : public WiimoteEventHandler {
	public:
		IAWiiMote();
		virtual ~IAWiiMote();

		void start();

		void close();

	private:
		[[noreturn]] void exec();

		/// Méthode appellée régulièrement pour traiter les entrées wiimote
		virtual void processWiimoteInput(WiimoteState& state) override;

		Interfacer::NavigationInterfacer& navigation();

		void advance(PhysicalRobot::SensAdvance, float magnitude = 1.0);

		void stop();

		void turn(PhysicalRobot::SensRotation, float magnitude = 1.0);

		void set_linear_speed(Speed);

		void set_angular_speed(AngularSpeed);

		TypeNavigation get_type_navigation(float angle_nunchuk);

		std::thread _execution;
		std::atomic_bool _is_running;

		std::shared_ptr<Interfacer::RobotManager> _manager;

		WiimoteInputProvider _input_provider;

		TypeNavigation _type_navigation = DO_NOTHING;

		float _angle_read;
		float _magnitude_read;
		float _magnitude_old = 0.0;

		bool _linear_velocity_pushed = false;
		bool _angular_velocity_pushed = false;

		bool _linear_velocity_very_fast = false;
		bool _angular_velocity_very_fast = false;
		bool _linear_velocity_fast = true;
		bool _angular_velocity_fast = true;

		const Vector2m _initial_position = toVec2(GLOBAL_CONSTANTS()["primary"].get_start_position());
		const Angle _initial_angle = GLOBAL_CONSTANTS()["primary"].get_start_angle();
	};
} // namespace Strategy

#endif // ROOT_IAWIIMOTE_H

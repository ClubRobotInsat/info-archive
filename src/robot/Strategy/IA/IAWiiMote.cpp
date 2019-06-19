//
// Created by terae on 04/02/19.
//

#include "IAWiiMote.h"

#define MANETTE_COUCHEE

extern void init_petri_navigation(std::shared_ptr<Strategy::Interfacer::RobotManager> manager, Constants::RobotColor color);

int main(int argc, char* argv[]) {
	Log::open(argc, argv, false);

	Strategy::IAWiiMote ia;
	ia.start();
}

namespace Strategy {

	using PhysicalRobot::SensAdvance;
	using PhysicalRobot::SensRotation;

	IAWiiMote::IAWiiMote() {
		auto robot = std::make_shared<PhysicalRobot::Robot>("primary", std::vector<std::string>(), Lidar::None);
		_manager = std::make_shared<Interfacer::RobotManager>(robot);
		Distance sx = 40_cm;
		Distance sy = 30_cm;
		Environment env({300, 200}, 1_cm, sy, (sqrt(sx * sx + sy * sy) / 2) * 1.2, Vector2m(0_m, 1_m));

		auto& avoidance = _manager->add_interfacer<Interfacer::AvoidanceInterfacer>(env);
		_manager->add_interfacer<Interfacer::NavigationInterfacer>(env, avoidance);

		try {
			init_petri_navigation(_manager, Constants::RobotColor::Purple);
		} catch(const std::exception& e) {
			logError("Error while initializing IAWiiMote: ", e.what());
			exit(1);
		}

		_input_provider.registerEventHandler(this);

		// navigation -> set_reference?
	}

	IAWiiMote::~IAWiiMote() {
		close();
	}

	void IAWiiMote::start() {
		if(_is_running) {
			_execution = std::thread(std::bind(&IAWiiMote::exec, this));
		}
	}

	void IAWiiMote::close() {
		_is_running = false;

		logDebug("End of IAWiiMote");
		_manager->get_robot()->deactivation();

		auto handle = _execution.native_handle();
		_execution.detach();
		pthread_cancel(handle);
		pthread_join(handle, nullptr);

		stop();
	}

	void IAWiiMote::exec() {
		setThreadName("IAWiiMote");
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

		_input_provider.startListening();
		logDebug("End connexion with wii mote");
		exit(0);
	}

	/// Méthode appellée régulièrement pour traiter les entrées wiimote
	void IAWiiMote::processWiimoteInput(WiimoteState& state) {
		if(state.hasNunchuk()) {
			state.getNunchukJoystickPosition(_angle_read, _magnitude_read);

			if(_magnitude_read > 0.2) {
				switch(get_type_navigation(_angle_read)) {
					case GO_FORWARD:
						advance(SensAdvance::Forward, _magnitude_read);
						break;
					case TURN_RIGHT:
						turn(SensRotation::Clockwise, _magnitude_read);
						break;
					case GO_BACKWARD:
						advance(SensAdvance::Backward, _magnitude_read);
						break;
					case TURN_LEFT:
						turn(SensRotation::Trigo, _magnitude_read);
						break;
					default:
						stop();
						break;
				}
			} else {
				stop();
			}
		} else {
// Soit on tient la manette comme dans mario kart, soit comme dans Wii Sport Resort
#ifdef MANETTE_COUCHEE
			if(state.isPressed(BUTTON_UP)) {
				turn(SensRotation::Clockwise);
			} else if(state.isPressed(BUTTON_LEFT)) {
				advance(SensAdvance::Backward);
			} else if(state.isPressed(BUTTON_RIGHT)) {
				advance(SensAdvance::Forward);
			} else if(state.isPressed(BUTTON_DOWN)) {
				turn(SensRotation::Trigo);
			} else {
				stop();
			}
#else
			if(state.isPressed(BUTTON_UP)) {
				advance(SensAdvance::Backward);
			} else if(state.isPressed(BUTTON_LEFT)) {
				turn(SensRotation::Trigo);
			} else if(state.isPressed(BUTTON_RIGHT)) {
				turn(SensRotation::Clockwise);
			} else if(state.isPressed(BUTTON_DOWN)) {
				advance(SensAdvance::Forward);
			} else {
				stop();
			}
#endif

#ifdef MANETTE_COUCHEE
			if(state.isPressed(BUTTON_B) || state.isPressed(BUTTON_PLUS)) {
				// TODO: use a servos' function F1
			} else if(state.isPressed(BUTTON_A) || state.isPressed(BUTTON_MINUS)) {
#else
			if(state.isPressed(BUTTON_1)) {
				// TODO: use a servos' function F1
			} else if(state.isPressed(BUTTON_2)) {
#endif
				// TODO: use a servos' function F2
			} else if(state.isPressed(BUTTON_HOME)) {
				// TODO: use a servos' function F3 OR goto_home()
			} else

// accélération - décélération
#ifdef MANETTE_COUCHEE
			    if(state.isPressed(BUTTON_2)) {
#else
			    if(state.isPressed(BUTTON_A)) {
#endif
				if(!_linear_velocity_very_fast) {
					if(!_linear_velocity_fast) {
						set_linear_speed(navigation().get_linear_speed() * 4);
						_linear_velocity_very_fast = true;
						_linear_velocity_fast = true;
					} else {
						set_linear_speed(navigation().get_linear_speed() * 2);
						_linear_velocity_very_fast = true;
					}
				}

				if(!_angular_velocity_very_fast) {
					if(!_angular_velocity_fast) {
						set_angular_speed(navigation().get_angular_speed() * 4);
						_angular_velocity_very_fast = true;
						_angular_velocity_fast = true;
					} else {
						set_angular_speed(navigation().get_angular_speed() * 2);
						_angular_velocity_very_fast = true;
					}
				}
#ifdef MANETTE_COUCHEE
			} else if(state.isPressed(BUTTON_1)) {
#else
			} else if(state.isPressed(BUTTON_B)) {
#endif
				if(_linear_velocity_very_fast) {
					set_linear_speed(navigation().get_linear_speed() * 0.25);
					_linear_velocity_very_fast = false;
					_linear_velocity_fast = false;
				} else if(_linear_velocity_fast) {
					set_linear_speed(navigation().get_linear_speed() * 0.5);
					_linear_velocity_fast = false;
				}

				if(_angular_velocity_very_fast) {
					set_angular_speed(navigation().get_angular_speed() * 0.25);
					_angular_velocity_very_fast = false;
					_angular_velocity_fast = false;
				} else if(_angular_velocity_fast) {
					set_angular_speed(navigation().get_angular_speed() * 0.5);
					_angular_velocity_fast = false;
				}
			} else {
				if(_angular_velocity_very_fast) {
					set_linear_speed(navigation().get_linear_speed() * 0.5);
					_angular_velocity_very_fast = false;
					_angular_velocity_fast = true;
				} else if(!_angular_velocity_fast) {
					set_linear_speed(navigation().get_linear_speed() * 2);
					_angular_velocity_very_fast = false;
					_angular_velocity_fast = true;
				}
				if(_angular_velocity_very_fast) {
					set_angular_speed(navigation().get_angular_speed() * 0.5);
					_angular_velocity_very_fast = false;
					_angular_velocity_fast = true;
				} else if(!_angular_velocity_fast) {
					set_angular_speed(navigation().get_angular_speed() * 2);
					_angular_velocity_very_fast = false;
					_angular_velocity_fast = true;
				}
			}
		}
	}

	Interfacer::NavigationInterfacer& IAWiiMote::navigation() {
		return _manager->get_interfacer<Interfacer::NavigationInterfacer>();
	}

	void IAWiiMote::advance(SensAdvance sens, float magnitude) {
		TypeNavigation type_nav = sens == SensAdvance::Forward ? GO_FORWARD : GO_BACKWARD;

		if(_type_navigation != DO_NOTHING && _type_navigation != type_nav) {
			stop();
		}
		if(_type_navigation == DO_NOTHING) {
			navigation().forward(10_m, sens, 1_ms);
			_magnitude_old = magnitude;
		} else if(!_linear_velocity_fast && magnitude >= 0.8) {
			set_linear_speed(navigation().get_linear_speed() * 2);
			_linear_velocity_fast = true;
		} else if(_linear_velocity_fast && magnitude < 0.8) {
			set_linear_speed(navigation().get_linear_speed() * 0.5);
			_linear_velocity_fast = false;
		}

		_type_navigation = type_nav;
	}

	void IAWiiMote::stop() {
		logDebug("IAWiiMote::stop()!");
		navigation().emergency_stop();
		_type_navigation = DO_NOTHING;
	}

	void IAWiiMote::turn(SensRotation sens, float magnitude) {
		TypeNavigation type_nav = sens == SensRotation::Clockwise ? TURN_RIGHT : TURN_LEFT;

		if(_type_navigation != DO_NOTHING && _type_navigation != type_nav) {
			stop();
		}
		if(_type_navigation == DO_NOTHING) {
			navigation().turn_relative(sens == SensRotation::Trigo ? 179_deg : -179_deg, 1_ms);
			std::cout << "tourner relatif" << std::endl;
			_magnitude_old = magnitude;
		} else if(!_angular_velocity_fast && magnitude >= 0.8) {
			set_angular_speed(navigation().get_angular_speed() * 2);
			_angular_velocity_fast = true;
		} else if(_angular_velocity_fast && magnitude < 0.8) {
			set_angular_speed(navigation().get_angular_speed() * 0.5);
			_angular_velocity_fast = false;
		}

		_type_navigation = type_nav;
	}

	void IAWiiMote::set_linear_speed(Speed speed) {
		if(_linear_velocity_pushed) {
			navigation().pop_linear_speed();
		}
		navigation().push_linear_speed(speed);
		_linear_velocity_pushed = true;
	}

	void IAWiiMote::set_angular_speed(AngularSpeed speed) {
		if(_angular_velocity_pushed) {
			navigation().pop_angular_speed();
		}
		navigation().push_angular_speed(speed);
		_angular_velocity_pushed = true;
	}

	TypeNavigation IAWiiMote::get_type_navigation(float angle_nunchuk) {
		if(angle_nunchuk >= 315 || angle_nunchuk <= 45)
			return GO_FORWARD;
		if(angle_nunchuk > 45 && angle_nunchuk < 135)
			return TURN_RIGHT;
		if(angle_nunchuk >= 135 && angle_nunchuk <= 225)
			return GO_BACKWARD;
		if(angle_nunchuk > 225 && angle_nunchuk < 315)
			return TURN_LEFT;

		return DO_NOTHING;
	}

} // namespace Strategy

#undef MANETTE_COUCHEE


#ifndef ROOT_IAPARSING_H
#define ROOT_IAPARSING_H

#include <Constants.h>

namespace Strategy {

	class IAArguments {
	public:
		IAArguments();

		IAArguments(int argc, char** argv);

		Constants::RobotColor get_color() const {
			return _color;
		}

		bool is_running_on_simulator() const {
			return _simulator;
		}

		bool should_exit() const {
			return _should_exit;
		}

		/** Write command usage on cout, then set _should_exit to true. */
		void print_help_and_exit();

		void print_parsed_arguments() const;

	private:
		bool _should_exit;

		Constants::RobotColor _color;
		bool _simulator;
	};

} // namespace Strategy

#endif // ROOT_IAPARSING_H

//
// Created by terae on 07/03/19.
//

#ifndef ROOT_TESTUTILS_H
#define ROOT_TESTUTILS_H

#include "../src/robot/Strategy/AbstractStrategy.h"

namespace TestUtils {
	static bool strategy_has_run;

	class StrategyTest final : public Strategy::AbstractStrategy {
	public:
		StrategyTest(Constants::RobotColor color = Constants::RobotColor::Yellow) : AbstractStrategy(color) {}

	private:
		void execute() override {
			strategy_has_run = true;
		}
	};

	class ModuleTest : public PhysicalRobot::Module {
	public:
		explicit ModuleTest(uint8_t id) : Module(id, "ModuleTest"), _a(1), _b(2), _deactivation_called(false) {}

		// Accesseurs pour les tests
		inline uint8_t get_a_value() const {
			return _a;
		}
		inline uint8_t get_b_value() const {
			return _b;
		}
		void set_a_value(int a) {
			_a = a;
			_state_changed.exchange(true);
		}
		void set_b_value(int b) {
			_b = b;
			_state_changed.exchange(true);
		}

		bool is_deactivation_called() const {
			return _deactivation_called;
		}

	private:
		std::vector<JSON> generate_list_jsons() const override {
			JSON j;
			j["a"] = _a.load();
			j["b"] = _b.load();

			return {j};
		}

		void message_processing(const JSON& j) override {
			_a.exchange(j["a"]);
			_b.exchange(j["b"]);
		}

		void deactivation() override {
			_deactivation_called = true;
		}

		std::atomic_uint8_t _a, _b;
		std::atomic_bool _deactivation_called;
	};
} // namespace TestUtils

#endif // ROOT_TESTUTILS_H

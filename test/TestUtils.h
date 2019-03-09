//
// Created by terae on 07/03/19.
//

#ifndef ROOT_TESTUTILS_H
#define ROOT_TESTUTILS_H

#include "../src/robot/Strategy/AbstractStrategy.h"
#ifdef PETRILAB_EXISTS
#include <petrilab/Cpp/MemberPetriDynamicLib.h>
#endif

extern void init_petri_tests(std::shared_ptr<Strategy::Interfacer::RobotManager> manager);

namespace TestUtils {

	class ModuleTest : public PhysicalRobot::Module {
	public:
		explicit ModuleTest(uint8_t id);

		// Accesseurs pour les tests
		uint8_t get_a_value() const;

		uint8_t get_b_value() const;

		void set_a_value(int a);

		void set_b_value(int b);

		bool is_deactivation_called() const;

	private:
		std::vector<JSON> generate_list_jsons() const override;

		void message_processing(const JSON& j) override;

		void deactivation() override;

		std::atomic_uint8_t _a, _b;
		std::atomic_bool _deactivation_called;
	};

	class InterfacerTest final : public Strategy::Interfacer::AbstractInterfacer {
	public:
		using interfaced_type = ModuleTest;

		explicit InterfacerTest(interfaced_type& module_tests);

		explicit InterfacerTest(std::shared_ptr<PhysicalRobot::Robot> robot);

		explicit InterfacerTest(PhysicalRobot::Robot& robot);

		interfaced_type* operator->();

	private:
		interfaced_type& _module;
	};

	class StrategyTest final : public Strategy::AbstractStrategy {
	public:
		StrategyTest(Constants::RobotColor color = Constants::RobotColor::Yellow);

		void debug_init();

		bool is_execute_called() const;

	private:
		void execute() override;

		std::atomic_bool _execute_called;
	};
} // namespace TestUtils

#endif // ROOT_TESTUTILS_H

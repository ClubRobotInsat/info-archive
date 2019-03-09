//
// Created by terae on 08/03/19.
//

#include "TestUtils.h"

extern void init_petri_tests(std::shared_ptr<Strategy::Interfacer::RobotManager> manager);

namespace TestUtils {

	ModuleTest::ModuleTest(uint8_t id) : Module(id, "ModuleTest"), _a(1), _b(2), _deactivation_called(false) {}

	// Accesseurs pour les tests
	uint8_t ModuleTest::get_a_value() const {
		return _a;
	}

	uint8_t ModuleTest::get_b_value() const {
		return _b;
	}

	void ModuleTest::set_a_value(int a) {
		_a = a;
		_state_changed.exchange(true);
	}

	void ModuleTest::set_b_value(int b) {
		_b = b;
		_state_changed.exchange(true);
	}

	bool ModuleTest::is_deactivation_called() const {
		return _deactivation_called;
	}

	std::vector<JSON> ModuleTest::generate_list_jsons() const {
		JSON j;
		j["a"] = _a.load();
		j["b"] = _b.load();

		return {j};
	}

	void ModuleTest::message_processing(const JSON& j) {
		_a.exchange(j["a"]);
		_b.exchange(j["b"]);
	}

	void ModuleTest::deactivation() {
		_deactivation_called = true;
	}

	InterfacerTest::InterfacerTest(interfaced_type& module_tests) : AbstractInterfacer(), _module(module_tests) {}

	InterfacerTest::InterfacerTest(std::shared_ptr<PhysicalRobot::Robot> robot) : InterfacerTest(*robot) {}

	InterfacerTest::InterfacerTest(PhysicalRobot::Robot& robot) : InterfacerTest(robot.get_module<interfaced_type>()) {}

	InterfacerTest::interfaced_type* InterfacerTest::operator->() {
		return &_module;
	}

	StrategyTest::StrategyTest(Constants::RobotColor color) : AbstractStrategy(color), _execute_called(false) {}

	void StrategyTest::debug_init() {
		auto modules_manager = std::make_shared<PhysicalRobot::ModuleManager>();
		modules_manager->add_module<ModuleTest>(5);

		auto physical_robot =
		    std::make_shared<PhysicalRobot::Robot>(modules_manager, std::vector({"StrategyTest"s, "NULL"s}), Lidar::None);

		auto interfacers_manager = std::make_shared<Strategy::Interfacer::RobotManager>(physical_robot);
		add_robot(physical_robot);
		init_petri_tests(interfacers_manager);

#if defined(PETRILAB_EXISTS) && false
		auto petrilab = Petri::Generated::Sample::createLib(".");
		petrilab->load();

		auto ia = petrilab->createPetriNet();
		ia->run();

		sleep(1_s);

		ia->stop();
#endif
	}

	bool StrategyTest::is_execute_called() const {
		return _execute_called;
	}

	void StrategyTest::execute() {
		_execute_called = true;
	}
} // namespace TestUtils

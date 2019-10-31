//
// Created by terae on 16/09/18.
//

#include "LED.h"
#include "IO.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
namespace PhysicalRobot {

	LED::LED(uint8_t id, uint8_t f)
	        : Module(id, "LED19")
	        , _freq_max(f)
	        , _state(PhysicalRobot::IOState::Off)
	        , _mode(PhysicalRobot::LEDMode::Auto)
	        , _auto_onoff(false)
	        , _pth(nullptr) {}

	LED::LED(uint8_t id)
	        : Module(id, "LED19")
	        , _state(PhysicalRobot::IOState::Off)
	        , _mode(PhysicalRobot::LEDMode::Manual)
	        , _auto_onoff(false)
	        , _pth(nullptr) {}

	std::vector<JSON> LED::generate_list_jsons() const {
		JSON j;
		j["state"] = toString(_state);
		return {j};
	}


	void wait_and_trig(LED* l) {
		while(l->get_auto_onoff()) {
			float e9 = 1000000000;
			int slp_tm = (int)(e9 / l->get_freq());
			std::this_thread::sleep_for(std::chrono::nanoseconds(slp_tm));
			l->trigger();
			// std::cout << l->getState() << std::endl;
		}
	}

	void LED::start() {
		if(_mode == LEDMode::Manual) {
			throw 20;
		}
		_auto_onoff = true;
		_pth = std::make_unique<std::thread>(wait_and_trig, this);
	}

	void LED::stop() {
		_auto_onoff = false;
	}

	void LED::on() {
		_state = IOState::On;
	}

	void LED::off() {
		_state = IOState::Off;
	}

	IOState LED::get_state() {
		return _state;
	}

	float LED::get_freq() {
		return _freq_max;
	}

	bool LED::get_auto_onoff() {
		return _auto_onoff;
	}


	void LED::trigger() {
		if(_state == PhysicalRobot::IOState::On)
			_state = PhysicalRobot::IOState::Off;
		else
			_state = PhysicalRobot::IOState::On;
	}

	LEDMode LED::get_mode() {
		return _mode;
	}


	LED::~LED() {

		if(_pth != nullptr)
			_pth->join();
	}

	void LED::message_processing(const JSON& j) {}

} // namespace PhysicalRobot

#pragma clang diagnostic pop
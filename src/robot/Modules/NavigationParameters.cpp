//
// Created by terae on 9/10/18.
//

#include "NavigationParameters.h"

#include "NavigationUtility.h"

namespace PhysicalRobot {

	void NavigationParameters::set_left_coder_radius(Distance radius) {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		_left_coder_radius = uint16_t(NavigationUtility::distance_to_i32(radius));
	}

	void NavigationParameters::set_right_wheel_coef(float coef) {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		_right_wheel_coef = coef;
	}

	void NavigationParameters::set_inter_axial_length(Distance length) {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		_inter_axial_length = uint16_t(NavigationUtility::distance_to_i32(length));
	}

	Distance NavigationParameters::get_left_wheel_radius() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return NavigationUtility::i32_to_distance(_left_coder_radius);
	}

	Distance NavigationParameters::get_right_wheel_radius() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return NavigationUtility::i32_to_distance(_left_coder_radius) * _right_wheel_coef;
	}

	float NavigationParameters::get_right_wheel_coef() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return _right_wheel_coef;
	}

	Distance NavigationParameters::get_inter_axial_length() const {
		std::lock_guard<std::mutex> lk(_mutex_variables);
		return NavigationUtility::i32_to_distance(_inter_axial_length);
	}

	void NavigationParameters::message_processing(const JSON&) {}

	void NavigationParameters::deactivation() {}

	uint32_t convertFloat(float value) {
		return uint32_t(doubleToFixedPoint(value, 16));
	}

	std::vector<JSON> NavigationParameters::generate_list_jsons() const {
		JSON result;
		result["coder_radius"] = _left_coder_radius;
		result["right_wheel_coef"] = convertFloat(_right_wheel_coef);
		result["inter_axial_length"] = _inter_axial_length;
		result["pos_kp"] = convertFloat(_pos_kp);
		result["pos_kd"] = convertFloat(_pos_kd);
		result["orient_kp"] = convertFloat(_orient_kp);
		result["orient_kd"] = convertFloat(_orient_kd);
		return {result};
	}

} // namespace PhysicalRobot

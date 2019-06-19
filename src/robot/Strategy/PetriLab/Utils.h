//
// Created by terae on 01/02/19.
//

#pragma once

#include <Constants.h>
#include <functional>

#define ADD_FN_CLASS(Vector, Class, Function, ...) Vector.push_back(std::bind(&Class::Function, this, ##__VA_ARGS__))
#define ADD_FN(Vector, Function, ...) Vector.push_back(std::bind(Function, ##__VA_ARGS__))

typedef std::function<Outcome(void)> fun_ar;

inline Outcome _combine_actions(std::vector<fun_ar> actions) {
	Outcome result = Outcome::SUCCESS;

	for(fun_ar action : actions) {
		if(result != Outcome::SUCCESS && result != Outcome::TIMEOUT)
			return result;
		result = action();
	}
	return result;
}

bool color_purple();
bool color_yellow();
bool color_undef();

Outcome set_points(int);
Outcome add_points(int);
int get_points();

Duration get_left_time();
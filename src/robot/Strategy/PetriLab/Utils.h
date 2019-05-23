//
// Created by terae on 01/02/19.
//

#pragma once

#include <Constants.h>
#include <functional>

#define ADD_FN_CLASS(Vector, Class, Function, ...) Vector.push_back(std::bind(&Class::Function, this, ##__VA_ARGS__))
#define ADD_FN(Vector, Function, ...) Vector.push_back(std::bind(Function, ##__VA_ARGS__))

typedef std::function<ActionResult(void)> fun_ar;

inline ActionResult _combine_actions(std::vector<fun_ar> actions) {
	ActionResult result = ActionResult::SUCCESS;

	for(fun_ar action : actions) {
		if(result != ActionResult::SUCCESS && result != ActionResult::TIMEOUT)
			return result;
		result = action();
	}
	return result;
}

bool color_purple();
bool color_yellow();
bool color_undef();

ActionResult set_points(int);
ActionResult add_points(int);
int get_points();

//
// Created by paul on 10/04/16.
//

#ifndef ROOT_SIMULATIONTOWEBSERVER_H
#define ROOT_SIMULATIONTOWEBSERVER_H

#endif // ROOT_SIMULATIONTOWEBSERVER_H
#include "petri/Runtime/Cpp/detail/jsoncpp/include/json/json.h"
#include <MathToolbox/MathToolbox.h>

using JSON = Json::Value;

inline JSON toJVector3(double x, double y, double z) {
	JSON value;
	// std::to_string n'est pas nécessaire en principe, mais quand une valeur vaut nan, sans to_string, elle est
	// convertie à 0 par la lib JSON.
	value["x"] = std::to_string(x);
	value["y"] = std::to_string(y);
	value["z"] = std::to_string(z);

	return value;
}

template <typename T>
JSON toJSON(Matrix4<T> const& mat) {
	JSON value;

	// Conversion column-major order vers row-major order.
	value.append(std::to_string(mat[0]));
	value.append(std::to_string(mat[4]));
	value.append(std::to_string(mat[8]));
	value.append(std::to_string(mat[12]));
	value.append(std::to_string(mat[1]));
	value.append(std::to_string(mat[5]));
	value.append(std::to_string(mat[9]));
	value.append(std::to_string(mat[13]));
	value.append(std::to_string(mat[2]));
	value.append(std::to_string(mat[6]));
	value.append(std::to_string(mat[10]));
	value.append(std::to_string(mat[14]));
	value.append(std::to_string(mat[3]));
	value.append(std::to_string(mat[7]));
	value.append(std::to_string(mat[11]));
	value.append(std::to_string(mat[15]));

	return value;
}
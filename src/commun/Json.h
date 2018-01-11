#ifndef ROOT_JSON_H
#define ROOT_JSON_H

#include <Color.h>
#include <MathToolbox/MathToolbox.h>
#include <petri/Runtime/Cpp/detail/jsoncpp/include/json/json.h>

using JSON = Json::Value;

namespace Json {
	inline Vector3m toVector3m(const Json::Value& json) {
		return {Distance::makeFromM(json["x"].asDouble()),
		        Distance::makeFromM(json["y"].asDouble()),
		        Distance::makeFromM(json["z"].asDouble())};
	}

	inline Vector3ang toVector3ang(const JSON& json) {
		return {Angle::makeFromDeg(json["x"].asDouble()),
		        Angle::makeFromDeg(json["y"].asDouble()),
		        Angle::makeFromDeg(json["z"].asDouble())};
	}

	inline Color3f toColor3f(const JSON& json) {
		return {json["r"].asFloat(), json["g"].asFloat(), json["b"].asFloat()};
	}

	inline JSON toJSONVector3(double x, double y, double z) {
		JSON value;
		// std::to_string n'est pas nécessaire en principe, mais quand une valeur vaut nan, sans to_string, elle est
		// convertie à 0 par la lib JSON.
		value["x"] = std::to_string(x);
		value["y"] = std::to_string(y);
		value["z"] = std::to_string(z);

		return value;
	}

	inline JSON toJSONVector3(const Vector3f& vec) {
		return toJSONVector3(vec.x, vec.y, vec.z);
	}

	template <typename T>
	inline JSON toJSONMatrix(Matrix4<T> const& mat) {
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

	/*JSON fromVector3m(const Vector3m & vec) {

	}*/
}
#endif // ROOT_JSON_H

#ifndef ROOT_JSON_H
#define ROOT_JSON_H

#include <Color.h>
#include <MathToolbox/MathToolbox.h>
#include <Units.h>
#include <json.hpp>

using JSON = nlohmann::json;

namespace Json {
	inline Vector2m toVector2m(const JSON& json) {
		return {Distance::makeFromM(json["x"].get<double>()), Distance::makeFromM(json["y"].get<double>())};
	}

	inline Vector3m toVector3m(const JSON& json) {
		return {Distance::makeFromM(json["x"].get<double>()),
		        Distance::makeFromM(json["y"].get<double>()),
		        Distance::makeFromM(json["z"].get<double>())};
	}

	inline Vector3ang toVector3ang(const JSON& json) {
		return {Angle::makeFromDeg(json["x"].get<double>()),
		        Angle::makeFromDeg(json["y"].get<double>()),
		        Angle::makeFromDeg(json["z"].get<double>())};
	}

	// TODO Rendre les conversions cohérentes vis-à-vis des erreurs qui peuvent survenir
	// Exemple : si un champ du json n'est pas convertible en float, cette fonction lève une LogicError
	// Voir le problème avec les NaN

	inline Color3f toColor3f(const JSON& json) {
		return {json["r"].get<float>(), json["g"].get<float>(), json["b"].get<float>()};
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

	inline JSON fromVector2m(const Vector2m& vec) {
		JSON jsonVec;
		jsonVec["x"] = vec.x.toM();
		jsonVec["y"] = vec.y.toM();
		return jsonVec;
	}

	inline JSON fromVector3m(const Vector3m& vec) {
		JSON jsonVec;
		jsonVec["x"] = vec.x.toM();
		jsonVec["y"] = vec.y.toM();
		jsonVec["z"] = vec.z.toM();
		return jsonVec;
	}

	inline JSON fromColor3f(const Color3f& color) {
		JSON value;
		value["r"] = color.r;
		value["g"] = color.g;
		value["b"] = color.b;
		return value;
	}

	template <typename T>
	inline JSON toJSONMatrix(Matrix4<T> const& mat) {
		JSON value;

		// Conversion column-major order vers row-major order.
		value.push_back(std::to_string(mat[0]));
		value.push_back(std::to_string(mat[4]));
		value.push_back(std::to_string(mat[8]));
		value.push_back(std::to_string(mat[12]));
		value.push_back(std::to_string(mat[1]));
		value.push_back(std::to_string(mat[5]));
		value.push_back(std::to_string(mat[9]));
		value.push_back(std::to_string(mat[13]));
		value.push_back(std::to_string(mat[2]));
		value.push_back(std::to_string(mat[6]));
		value.push_back(std::to_string(mat[10]));
		value.push_back(std::to_string(mat[14]));
		value.push_back(std::to_string(mat[3]));
		value.push_back(std::to_string(mat[7]));
		value.push_back(std::to_string(mat[11]));
		value.push_back(std::to_string(mat[15]));

		return value;
	}
}
#endif // ROOT_JSON_H

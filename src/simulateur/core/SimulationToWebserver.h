//
// Created by paul on 10/04/16.
//

#ifndef ROOT_SIMULATIONTOWEBSERVER_H
#define ROOT_SIMULATIONTOWEBSERVER_H

#include <iomanip>
#include <sstream>

#include <MathToolbox/MathToolbox.h>
#include <Units.h>
#include <petri/Runtime/Cpp/detail/jsoncpp/include/json/json.h>

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

inline JSON toJVector3(const Vector3f& vec) {
	return toJVector3(vec.x, vec.y, vec.z);
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

/** Transforme une couleur représentée sur des floats (entre 0 et 1)
 * en couleur hexadecimale. */
inline std::string toHexColorStr(const Vector3f& vec) {
	unsigned char r = (unsigned char)(vec.r * 255.99);
	unsigned char g = (unsigned char)(vec.g * 255.99);
	unsigned char b = (unsigned char)(vec.b * 255.99);

	std::ostringstream stream;
	stream << "0x" << std::hex << std::setw(2) << std::setfill('0') << (int)r << std::setw(2) << std::setfill('0')
	       << (int)g << std::setw(2) << std::setfill('0') << (int)b;
	return stream.str();
}

inline float toWebServer(const Length& l) {
	return (float)l.toDm();
}

inline Vector3f toWebServer(const Vector3m& vec) {
	return {toWebServer(vec.x), toWebServer(vec.y), toWebServer(vec.z)};
}

inline Vector3f toWebServer(const Vector3ang& vec) {
	return {(float)vec.x.toDeg(), (float)vec.y.toDeg(), (float)vec.z.toDeg()};
}

// TODO float conversion ?

#endif // ROOT_SIMULATIONTOWEBSERVER_H
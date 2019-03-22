// PythonCartesCommun.cpp - exposition à l'API Python de la partie commune des cartes des différentes années

#include "CarteDeplacementCommun.h"
#include <boost/python.hpp>

using namespace boost::python;

///
/// Module CartesCommun - les lignes qui suivent créent la fonction initCartesCommun() qui crée le module Python
/// CartesCommun
///

BOOST_PYTHON_MODULE(CartesCommun) {
	class_<Coordonnees>("Coordonnees").def_readwrite("x", &Coordonnees::x).def_readwrite("y", &Coordonnees::y).def_readwrite("theta", &Coordonnees::theta);

	enum_<SensAvance>("SensAvance").value("SENS_ARRIERE", SENS_ARRIERE).value("SENS_AVANT", SENS_AVANT);

	enum_<SensRotation>("SensRotation").value("SENS_HORAIRE", SENS_HORAIRE).value("SENS_TRIGO", SENS_TRIGO);
}

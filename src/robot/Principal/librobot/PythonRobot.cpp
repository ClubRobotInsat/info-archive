/*
 * PythonRobotPrincipal2015.cpp
 *
 */

// PythonAPI.cpp - ici est regroupée toute la déclaration de l'API Python de la lib robot.

#include "MecaManager2015.h"
#include "Robot2015.h"

#include "CarteExemple.h"

#include <boost/python.hpp>

using namespace boost::python;
using namespace IDCartesPrincipal2015;
using namespace ConstantesPrincipal2015;
using namespace Constantes2015;

// Module RobotPrincipal2015 - les lignes qui suivent créent la fonction initRobotPrincipal2015() qui crée le module
// Python Robot2015.
BOOST_PYTHON_MODULE(RobotPrincipal2015) {
	class_<RobotPrincipal2015, bases<Robot>>("RobotPrincipal2015", init<const char*, const char*, int, int, int>())
	    .def("assignerCarte", &Robot::assignerCarte)
	    .def("getCarte", &Robot::getCarte, return_value_policy<reference_existing_object>())
	    .def("getCarteDeplacement", &RobotPrincipal2015::getCarteDeplacement, return_value_policy<reference_existing_object>())
	    .def("getCarteAscenseur", &RobotPrincipal2015::getCarteAscenseur, return_value_policy<reference_existing_object>())
	    .def("getCarteIO", &RobotPrincipal2015::getCarteIO, return_value_policy<reference_existing_object>())
	    .def("getCartePneumatique", &RobotPrincipal2015::getCartePneumatique, return_value_policy<reference_existing_object>());
}

BOOST_PYTHON_MODULE(MecaManager2015) {
	class_<MecaManager2015>("MecaManager2015", init<RobotPrincipal2015&, CouleurRobot>());
}


BOOST_PYTHON_MODULE(Constantes2015) {
	enum_<CouleurRobot>("CouleurRobot").value("COULEUR_ROBOT_VERT", COULEUR_ROBOT_VERT).value("COULEUR_ROBOT_JAUNE", COULEUR_ROBOT_JAUNE);

	// Pour une variable
	// boost::python::scope().attr("vitesseRotationMoissoneuse") = vitesseRotationMoissoneuse;
}

BOOST_PYTHON_MODULE(ConstantesPrincipal2015Type) {
	;
}

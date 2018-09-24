// PythonCartes2007.cpp - ici est regroupée la déclaration de l'API Python de la lib carte pour les cartes 2007.
#include "WrapperCartes2007.h"
#include <boost/python.hpp>

using namespace boost::python;

///
/// Module Cartes2007 - les lignes qui suivent créent la fonction initCartes2007() qui crée le module Python Cartes2007.
///

BOOST_PYTHON_MODULE(Cartes2007) {
	class_<CarteActionneurs2007Wrap, boost::noncopyable>("CarteActionneurs2007")
	    .def("traiterMessage", &CarteActionneurs2007::traiterMessage, &CarteActionneurs2007Wrap::default_traiterMessage);

	enum_<CarteDeplacement2007::TypeAck>("TypeAck")
	    .value("ACK_RIEN", CarteDeplacement2007::ACK_RIEN)
	    .value("ACK_MESSAGE_EXECUTE", CarteDeplacement2007::ACK_MESSAGE_EXECUTE)
	    .value("ACK_MESSAGE_IGNORE", CarteDeplacement2007::ACK_MESSAGE_IGNORE);

	enum_<CarteDeplacement2007::TypeMesure>("TypeMesure")
	    .value("MESURE_DISTANCE", CarteDeplacement2007::MESURE_DISTANCE)
	    .value("MESURE_ANGLE", CarteDeplacement2007::MESURE_ANGLE)
	    .value("MESURE_DROITE", CarteDeplacement2007::MESURE_DROITE)
	    .value("MESURE_GAUCHE", CarteDeplacement2007::MESURE_GAUCHE);

	enum_<CarteDeplacement2007::TypeGrandeur>("TypeGrandeur")
	    .value("POSITION", CarteDeplacement2007::POSITION)
	    .value("ANGLE", CarteDeplacement2007::ANGLE)
	    .value("DROITE", CarteDeplacement2007::DROITE)
	    .value("GAUCHE", CarteDeplacement2007::GAUCHE)
	    .value("VPOSITION", CarteDeplacement2007::VPOSITION)
	    .value("VANGLE", CarteDeplacement2007::VANGLE);

	class_<CarteDeplacement2007>("CarteDeplacement2007")
	    .def("EnvoyerPing", &CarteDeplacement2007::EnvoyerPing)
	    .def("verifierPong", &CarteDeplacement2007::verifierPong)
	    .def("traiterMessage", &CarteDeplacement2007::traiterMessage)
	    .def("verifierBlocage", &CarteDeplacement2007::verifierBlocage)
	    .def("DefinirCoordonnees", &CarteDeplacement2007::DefinirCoordonnees)
	    .def("arreter", &CarteDeplacement2007::arreter)
	    .def("Avancer", &CarteDeplacement2007::Avancer)
	    .def("Tourner", &CarteDeplacement2007::Tourner)
	    .def("TournerAbsolu", &CarteDeplacement2007::TournerAbsolu)
	    .def("PasserPar", &CarteDeplacement2007::PasserPar)
	    .def("pointerVers", &CarteDeplacement2007::pointerVers)
	    .def("VaVers", &CarteDeplacement2007::VaVers);
}

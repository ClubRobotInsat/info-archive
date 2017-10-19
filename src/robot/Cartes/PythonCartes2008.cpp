// PythonCartes2008.cpp - ici est regroupée la déclaration de l'API Python de la lib carte pour les cartes 2008.
#include "WrapperCartes2008.h"
#include <boost/python.hpp>

using namespace boost::python;

///
/// Module Cartes2008 - les lignes qui suivent créent la fonction initCartes2008() qui crée le module Python Cartes2008.
///

BOOST_PYTHON_MODULE(Cartes2008) {
	class_<CarteAsservissement2008Wrap, boost::noncopyable>("CarteAsservissement2008")
	    .def("traiterMessage", &CarteAsservissement2008::traiterMessage, &CarteAsservissement2008Wrap::default_traiterMessage);

	class_<CarteTelemetreIR2008Wrap, boost::noncopyable>("CarteTelemetreIR2008")
	    .def("traiterMessage", &CarteTelemetreIR2008::traiterMessage, &CarteTelemetreIR2008Wrap::default_traiterMessage)
	    .def("EnvoyerPing", &CarteTelemetreIR2008::EnvoyerPing)
	    .def("verifierPong", &CarteTelemetreIR2008::verifierPong)
	    .def("ActualiserTelemetre", &CarteTelemetreIR2008::ActualiserTelemetre)
	    .def("TelemetreActualise", &CarteTelemetreIR2008::TelemetreActualise)
	    .def("LireTelemetre", &CarteTelemetreIR2008::LireTelemetre)
	    .def("ActiverEnvoiAuto", &CarteTelemetreIR2008::ActiverEnvoiAuto)
	    .def("DesactiverEnvoiAuto", &CarteTelemetreIR2008::DesactiverEnvoiAuto);
}

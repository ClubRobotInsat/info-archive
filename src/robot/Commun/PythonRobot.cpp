// PythonRobot.cpp - ici est regroupée toute la déclaration de l'API Python de la libRobot.
#include <boost/python.hpp>

#include "Carte.h"
#include "Robot.h"

using namespace boost::python;

///
/// Wrapper autour de Carte
///

class CarteWrap : public Carte, public wrapper<Carte> {
public:
	virtual void traiterMessage(unsigned char cmd, unsigned char taille, uint8_t* donnees) override {
		this->get_override("traiterMessage")(cmd, taille, donnees);
	}
};

///
/// Module Robot - les lignes qui suivent créent la fonction initRobot() qui crée le module Python Robot.
///

void (Robot::*Robot_Connecter1)(const char*, const char*, int) = &Robot::connecter;
void (Robot::*Robot_Connecter2)(int, char**) = &Robot::connecter;

BOOST_PYTHON_MODULE(Robot) {
	class_<CarteWrap, boost::noncopyable>("Carte").def("assigner", &Carte::assigner).def("traiterMessage", pure_virtual(&Carte::traiterMessage));

	class_<Robot>("Robot", init<int, char**>())
	    .def(init<const char*>())
	    .def(init<const char*, const char*>())
	    .def(init<const char*, const char*, int>())

	    .def("assignerCarte", &Robot::assignerCarte)
	    .def("Connecter", Robot_Connecter1)
	    .def("Connecter", Robot_Connecter2)
	    .def("getCarte", &Robot::getCarte, return_value_policy<reference_existing_object>())
	    .def("setDebugCAN", &Robot::setDebugCAN);
}

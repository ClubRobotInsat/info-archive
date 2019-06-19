// PythonTerm.cpp - terminal permettant de controler le robot à la main en python

//#include <Python.h >
#include <boost/python.hpp>
using namespace boost::python;

extern "C" {
// Implémentées dans les fichiers PythonXXXXXXX.cpp
// Ces fonctions créent les modules éponymes dans l'interpréteur Python ("Robot", "Cartes2009"...).
#ifdef WIN32
#define ROBOT_API __declspec(dllexport)
#else
#define ROBOT_API
#endif

void ROBOT_API initRobot();
void ROBOT_API initCartesCommun();
void ROBOT_API initCartes2015();
void ROBOT_API initRobotPrincipal2015();
void ROBOT_API initConstantes2015();
void ROBOT_API initConstantesPrincipal2015();
void ROBOT_API initMecaManager2015();
}

int main(int argc, char* argv[]) {
	// initialisation de l'interpreteur python
	Py_Initialize();

	// initialisation des modules
	initRobot();
	initCartesCommun();
	initCartes2015();
	initRobotPrincipal2015();
	initMecaManager2015();
	initConstantes2015();
	initConstantesPrincipal2015();

	// importation du contenu des modules
	PyRun_SimpleString("from Robot import *");
	PyRun_SimpleString("from CartesCommun import *");
	PyRun_SimpleString("from Cartes2015 import *");
	PyRun_SimpleString("from RobotPrincipal2015 import *");
	PyRun_SimpleString("from MecaManager2015 import *");
	PyRun_SimpleString("from Constantes2015 import *");
	PyRun_SimpleString("from ConstantesPrincipal2015 import *");

	// lancement du terminal
	Py_Main(argc, argv);

	// arret de l'interpreteur python
	Py_Finalize();

	return 0;
}

/*
 * PythonCartes2014.cpp
 *
 * Author: sivadier
 */

// PythonCartes2014.cpp - ici est regroupée la déclaration de l'API Python
// de la lib carte pour les cartes 2014.
#include "WrapperCartes2009.h"
#include <boost/python.hpp>

using namespace boost::python;

///
/// Module Cartes2014 - les lignes qui suivent créent la fonction initCartes2014()
/// qui crée le module Python Cartes2014.
///

BOOST_PYTHON_MODULE(Cartes2014) {
	enum_<CarteDeplacement2009::TypeAck>("TypeAck")
	    .value("ACK_RIEN", CarteDeplacement2009::ACK_RIEN)
	    .value("ACK_MESSAGE_EXECUTE", CarteDeplacement2009::ACK_MESSAGE_EXECUTE)
	    .value("ACK_MESSAGE_IGNORE", CarteDeplacement2009::ACK_MESSAGE_IGNORE);

	enum_<CarteDeplacement2009::TypeMesure>("TypeMesure")
	    .value("MESURE_POSITION", CarteDeplacement2009::MESURE_POSITION)
	    .value("MESURE_VITESSE_CONSIGNE", CarteDeplacement2009::MESURE_VITESSE_CONSIGNE)
	    .value("MESURE_VITESSE_MESUREE", CarteDeplacement2009::MESURE_VITESSE_MESUREE)
	    .value("MESURE_COMMANDE", CarteDeplacement2009::MESURE_COMMANDE);

	enum_<CarteDeplacement2009::TypeGrandeur>("TypeGrandeur")
	    .value("POSITION", CarteDeplacement2009::POSITION)
	    .value("ANGLE", CarteDeplacement2009::ANGLE)
	    .value("DROITE", CarteDeplacement2009::DROITE)
	    .value("GAUCHE", CarteDeplacement2009::GAUCHE)
	    .value("VPOSITION", CarteDeplacement2009::VPOSITION)
	    .value("VANGLE", CarteDeplacement2009::VANGLE);

	class_<CarteDeplacement2009>("CarteDeplacement2009")
	    .def("reset", &CarteDeplacement2009::reset)
	    .def("envoyerPing", &CarteDeplacement2009::envoyerPing)
	    .def("verifierPong", &CarteDeplacement2009::verifierPong)
	    .def("actualiserCoordonnees", &CarteDeplacement2009::actualiserCoordonnees)
	    .def("coordonneesActualisees", &CarteDeplacement2009::coordonneesActualisees)
	    .def("lireCoordonnees", &CarteDeplacement2009::lireCoordonnees)
	    .def("definirCoordonnees", &CarteDeplacement2009::definirCoordonnees)
	    .def("verifierDeplacementTermine", &CarteDeplacement2009::verifierDeplacementTermine)
	    .def("verifierPrecisionAtteinte", &CarteDeplacement2009::verifierPrecisionAtteinte)
	    .def("arreter", &CarteDeplacement2009::arreter)
	    .def("avancer", &CarteDeplacement2009::avancer)
	    .def("tourner", &CarteDeplacement2009::tourner)
	    .def("tournerAbsolu", &CarteDeplacement2009::tournerAbsolu)
	    .def("pointerVers", &CarteDeplacement2009::pointerVers)
	    .def("vaVers", &CarteDeplacement2009::vaVers)
	    .def("allerADecompose", &CarteDeplacement2009::allerADecompose)
	    .def("allerA", &CarteDeplacement2009::allerA)
	    .def("avancerInfini", &CarteDeplacement2009::avancerInfini)
	    .def("tournerInfini", &CarteDeplacement2009::tournerInfini)
	    .def("passerPar", &CarteDeplacement2009::passerPar)
	    .def("pivoterDroite", &CarteDeplacement2009::pivoterDroite)
	    .def("pivoterGauche", &CarteDeplacement2009::pivoterGauche)
	    .def("positionDroiteGauche", &CarteDeplacement2009::positionDroiteGauche)
	    .def("vitesseDroiteGauche", &CarteDeplacement2009::vitesseDroiteGauche)
	    .def("arretUrgence", &CarteDeplacement2009::arretUrgence)
	    .def("controleVitesseLongAng", &CarteDeplacement2009::controleVitesseLongAng)
	    .def("demanderMesure", &CarteDeplacement2009::demanderMesure)
	    .def("mesureActualisee", &CarteDeplacement2009::mesureActualisee)
	    .def("lireMesure", &CarteDeplacement2009::lireMesure)
	    .def("allumerLed", &CarteDeplacement2009::allumerLed)
	    .def("activerAsservissement", &CarteDeplacement2009::activerAsservissement)
	    .def("ackRecu", &CarteDeplacement2009::ackRecu)
	    .def("activerEnvoiAuto", &CarteDeplacement2009::activerEnvoiAuto)
	    .def("reglerParametre", &CarteDeplacement2009::reglerParametre)
	    .def("demanderParametre", &CarteDeplacement2009::demanderParametre)
	    .def("parametreActualisee", &CarteDeplacement2009::parametreActualisee)
	    .def("lireParametre", &CarteDeplacement2009::lireParametre);


	class_<CarteAsservissement2009>("CarteAsservissement2009")
	    .def("reset", &CarteAsservissement2009::reset)
	    .def("envoyerPing", &CarteAsservissement2009::envoyerPing)
	    .def("verifierPong", &CarteAsservissement2009::verifierPong)
	    .def("actualiserAngle", &CarteAsservissement2009::actualiserAngle)
	    .def("angleActualise", &CarteAsservissement2009::angleActualise)
	    .def("lireAngle", &CarteAsservissement2009::lireAngle)
	    .def("definirAngle", &CarteAsservissement2009::definirAngle)
	    .def("verifierDeplacementTermine", &CarteAsservissement2009::verifierDeplacementTermine)
	    .def("verifierPrecisionAtteinte", &CarteAsservissement2009::definirAngle)
	    .def("verifierBlocage", &CarteAsservissement2009::verifierBlocage)
	    .def("arreter", &CarteAsservissement2009::arreter)
	    .def("tourner", &CarteAsservissement2009::tourner)
	    .def("tournerAbsolu", &CarteAsservissement2009::tournerAbsolu)
	    .def("tournerInfini", &CarteAsservissement2009::tournerInfini)
	    .def("arretUrgence", &CarteAsservissement2009::arretUrgence)
	    .def("demanderMesure", &CarteAsservissement2009::demanderMesure)
	    .def("mesureActualisee", &CarteAsservissement2009::mesureActualisee)
	    .def("lireMesure", &CarteAsservissement2009::lireMesure)
	    .def("allumerLed", &CarteAsservissement2009::allumerLed)
	    .def("activerAsservissementAngle", &CarteAsservissement2009::activerAsservissementAngle)
	    //.def("AckRecu", &CarteAsservissement2009::AckRecu)
	    .def("activerEnvoiAuto", &CarteAsservissement2009::activerEnvoiAuto)
	    .def("reglerParametre", &CarteAsservissement2009::reglerParametre)
	    .def("demanderParametre", &CarteAsservissement2009::demanderParametre)
	    .def("parametreActualise", &CarteAsservissement2009::parametreActualise)
	    .def("lireParametre", &CarteAsservissement2009::lireParametre)
	    .def("tensionMoteur", &CarteAsservissement2009::tensionMoteur);

	class_<CarteServosNova2017>("CarteServosNova2017")
	    .def("envoyerPing", &CarteServosNova2017::envoyerPing)
	    .def("verifierPong", &CarteServosNova2017::verifierPong)
	    .def("reglerPosition", &CarteServosNova2017::reglerPosition)
	    .def("actualiserPosition", &CarteServosNova2017::actualiserPosition)
	    .def("verifierPositionActualisee", &CarteServosNova2017::verifierPositionActualisee)
	    .def("lirePosition", &CarteServosNova2017::lirePosition)
	    .def("verifierDeplacementTermine", &CarteServosNova2017::verifierDeplacementTermine)
	    .def("verifierServoBloque", &CarteServosNova2017::verifierServoBloque);

	class_<CarteContacteurs2007>("CarteContacteurs2007")
	    .def("envoyerPing", &CarteContacteurs2007::envoyerPing)
	    .def("verifierPong", &CarteContacteurs2007::verifierPong)
	    .def("actualiserContacteurs", &CarteContacteurs2007::actualiserContacteurs)
	    .def("sontContacteursActualises", &CarteContacteurs2007::sontContacteursActualises)
	    .def("getContacteur", &CarteContacteurs2007::getContacteur);

	class_<CarteIHM>("CarteIHM")
	    .def("envoyerPing", &CarteIHM::envoyerPing)
	    .def("verifierPong", &CarteIHM::verifierPong)
	    .def("actualiserContacteurs", &CarteIHM::actualiserContacteurs)
	    .def("sontContacteursActualises", &CarteIHM::sontContacteursActualises)
	    .def("getContacteur", &CarteIHM::getContacteur);

	class_<CartePneumatique2014>("CartePneumatique2014")
	    .def("envoyerPing", &CartePneumatique2014::envoyerPing)
	    .def("verifierPong", &CartePneumatique2014::verifierPong)
	    .def("activerPompe", &CartePneumatique2014::activerPompe)
	    .def("desactiverPompe", &CartePneumatique2014::desactiverPompe)
	    .def("getPompeActivee", &CartePneumatique2014::getPompeActivee);

	class_<CarteIO2014>("CarteIO2014")
	    .def("envoyerPing", &CarteIO2014::envoyerPing)
	    .def("verifierPong", &CarteIO2014::verifierPong)
	    .def("actualiserPuisLireUnCapteur", &CarteIO2014::actualiserPuisLireUnCapteur)
	    .def("actualiserUnCapteur", &CarteIO2014::actualiserUnCapteur)
	    .def("lireUnCapteur", &CarteIO2014::lireUnCapteur)
	    .def("capteurEstActualise", &CarteIO2014::capteurEstActualise);

	class_<CarteAlimentation2010>("CarteAlimentation2010")
	    .def("envoyerPing", &CarteAlimentation2010::envoyerPing)
	    .def("verifierPong", &CarteAlimentation2010::verifierPong)
	    .def("getTension", &CarteAlimentation2010::getTension);

	/* class_<CarteEvitement2012>("CarteEvitement2012", init<ICapteurs2012*>())
	.def("EnvoyerPing", &CarteAlimentation2010::EnvoyerPing)
	.def("verifierPong", &CarteAlimentation2010::verifierPong)
	.def("DemandeEstPret", &CarteEvitement2012::DemandeEstPret)
	.def("getEstPret", &CarteEvitement2012::getEstPret)
	.def("DemandePositionAdv", &CarteEvitement2012::DemandePositionAdv)
	.def("getPositionAdversaireX", &CarteEvitement2012::getPositionAdversaireX)
	.def("getPositionAdversaireY", &CarteEvitement2012::getPositionAdversaireY)
	.def("SetConfig", &CarteEvitement2012::setConfig)
	;*/

	class_<CarteDetectAdv2009, bases<CarteAsservissement2009>>("CarteDetectAdv2009")
	    .def("actualiserMesureAdv", &CarteDetectAdv2009::actualiserMesureAdv)
	    .def("verifierMesureAdvActualise", &CarteDetectAdv2009::verifierMesureAdvActualise)
	    .def("getDiametreAngulaire", &CarteDetectAdv2009::getDiametreAngulaire)
	    .def("getAngle", &CarteDetectAdv2009::getAngle)
	    .def("activerEnvoiMesureAdvAuto", &CarteDetectAdv2009::activerEnvoiMesureAdvAuto)
	    .def("desactiverEnvoiMesureAdvAuto", &CarteDetectAdv2009::desactiverEnvoiMesureAdvAuto);
}

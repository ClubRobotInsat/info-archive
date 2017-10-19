// PythonCartes2009.cpp - ici est regroupée la déclaration de l'API Python de la lib carte pour les cartes 2009.
#include "WrapperCartes2009.h"
#include <boost/python.hpp>

using namespace boost::python;

///
/// Module Cartes2009 - les lignes qui suivent créent la fonction initCartes2009() qui crée le module Python Cartes2009.
///

BOOST_PYTHON_MODULE(Cartes2009) {
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
	    .def("Reset", &CarteDeplacement2009::Reset)
	    .def("EnvoyerPing", &CarteDeplacement2009::EnvoyerPing)
	    .def("verifierPong", &CarteDeplacement2009::verifierPong)
	    .def("ActualiserCoordonnees", &CarteDeplacement2009::ActualiserCoordonnees)
	    .def("coordonneesActualisees", &CarteDeplacement2009::coordonneesActualisees)
	    .def("LireCoordonnees", &CarteDeplacement2009::LireCoordonnees)
	    .def("DefinirCoordonnees", &CarteDeplacement2009::DefinirCoordonnees)
	    .def("verifierDeplacementTermine", &CarteDeplacement2009::verifierDeplacementTermine)
	    .def("verifierPrecisionAtteinte", &CarteDeplacement2009::verifierPrecisionAtteinte)
	    .def("arreter", &CarteDeplacement2009::arreter)
	    .def("Avancer", &CarteDeplacement2009::Avancer)
	    .def("Tourner", &CarteDeplacement2009::Tourner)
	    .def("TournerAbsolu", &CarteDeplacement2009::TournerAbsolu)
	    .def("pointerVers", &CarteDeplacement2009::pointerVers)
	    .def("VaVers", &CarteDeplacement2009::VaVers)
	    .def("allerADecompose", &CarteDeplacement2009::allerADecompose)
	    .def("allerA", &CarteDeplacement2009::allerA)
	    .def("avancerInfini", &CarteDeplacement2009::avancerInfini)
	    .def("tournerInfini", &CarteDeplacement2009::tournerInfini)
	    .def("PasserPar", &CarteDeplacement2009::PasserPar)
	    .def("PivoterDroite", &CarteDeplacement2009::PivoterDroite)
	    .def("PivoterGauche", &CarteDeplacement2009::PivoterGauche)
	    .def("PositionDroiteGauche", &CarteDeplacement2009::PositionDroiteGauche)
	    .def("VitesseDroiteGauche", &CarteDeplacement2009::VitesseDroiteGauche)
	    .def("ArretUrgence", &CarteDeplacement2009::ArretUrgence)
	    .def("ControleVitesseLongAng", &CarteDeplacement2009::ControleVitesseLongAng)
	    .def("DemanderMesure", &CarteDeplacement2009::DemanderMesure)
	    .def("MesureActualisee", &CarteDeplacement2009::MesureActualisee)
	    .def("LireMesure", &CarteDeplacement2009::LireMesure)
	    .def("AllumerLed", &CarteDeplacement2009::AllumerLed)
	    .def("ActiverAsservissement", &CarteDeplacement2009::ActiverAsservissement)
	    .def("AckRecu", &CarteDeplacement2009::AckRecu)
	    .def("ActiverEnvoiAuto", &CarteDeplacement2009::ActiverEnvoiAuto)
	    .def("ReglerParametre", &CarteDeplacement2009::ReglerParametre)
	    .def("DemanderParametre", &CarteDeplacement2009::DemanderParametre)
	    .def("ParametreActualisee", &CarteDeplacement2009::ParametreActualisee)
	    .def("LireParametre", &CarteDeplacement2009::LireParametre);

	class_<CarteTelemetres2009>("CarteTelemetres2009")
	    .def("traiterMessage", &CarteTelemetres2009::traiterMessage)
	    .def("EnvoyerPing", &CarteTelemetres2009::EnvoyerPing)
	    .def("verifierPong", &CarteTelemetres2009::verifierPong)
	    .def("ActualiserTelemetre", &CarteTelemetres2009::ActualiserTelemetre)
	    .def("TelemetreActualise", &CarteTelemetres2009::TelemetreActualise)
	    .def("LireDistanceTelemetre", &CarteTelemetres2009::LireDistanceTelemetre)
	    .def("ActiverEnvoiAuto", &CarteTelemetres2009::ActiverEnvoiAuto)
	    .def("DesactiverEnvoiAuto", &CarteTelemetres2009::DesactiverEnvoiAuto);

	class_<CarteAsservissement2009>("CarteAsservissement2009")
	    .def("Reset", &CarteAsservissement2009::Reset)
	    .def("EnvoyerPing", &CarteAsservissement2009::EnvoyerPing)
	    .def("verifierPong", &CarteAsservissement2009::verifierPong)
	    .def("ActualiserAngle", &CarteAsservissement2009::ActualiserAngle)
	    .def("AngleActualise", &CarteAsservissement2009::AngleActualise)
	    .def("LireAngle", &CarteAsservissement2009::LireAngle)
	    .def("DefinirAngle", &CarteAsservissement2009::DefinirAngle)
	    .def("verifierDeplacementTermine", &CarteAsservissement2009::verifierDeplacementTermine)
	    .def("verifierPrecisionAtteinte", &CarteAsservissement2009::DefinirAngle)
	    .def("verifierBlocage", &CarteAsservissement2009::verifierBlocage)
	    .def("arreter", &CarteAsservissement2009::arreter)
	    .def("Tourner", &CarteAsservissement2009::Tourner)
	    .def("TournerAbsolu", &CarteAsservissement2009::TournerAbsolu)
	    .def("tournerInfini", &CarteAsservissement2009::tournerInfini)
	    .def("ArretUrgence", &CarteAsservissement2009::ArretUrgence)
	    .def("DemanderMesure", &CarteAsservissement2009::DemanderMesure)
	    .def("MesureActualisee", &CarteAsservissement2009::MesureActualisee)
	    .def("LireMesure", &CarteAsservissement2009::LireMesure)
	    .def("AllumerLed", &CarteAsservissement2009::AllumerLed)
	    .def("ActiverAsservissementAngle", &CarteAsservissement2009::ActiverAsservissementAngle)
	    //.def("AckRecu", &CarteAsservissement2009::AckRecu)
	    .def("ActiverEnvoiAuto", &CarteAsservissement2009::ActiverEnvoiAuto)
	    .def("ReglerParametre", &CarteAsservissement2009::ReglerParametre)
	    .def("DemanderParametre", &CarteAsservissement2009::DemanderParametre)
	    .def("ParametreActualise", &CarteAsservissement2009::ParametreActualise)
	    .def("LireParametre", &CarteAsservissement2009::LireParametre)
	    .def("TensionMoteur", &CarteAsservissement2009::TensionMoteur);

	class_<CarteDetectAdv2009, bases<CarteAsservissement2009>>("CarteDetectAdv2009")
	    .def("ActualiserMesureAdv", &CarteDetectAdv2009::ActualiserMesureAdv)
	    .def("verifierMesureAdvActualise", &CarteDetectAdv2009::verifierMesureAdvActualise)
	    .def("getDiametreAngulaire", &CarteDetectAdv2009::getDiametreAngulaire)
	    .def("getAngle", &CarteDetectAdv2009::getAngle)
	    .def("ActiverEnvoiMesureAdvAuto", &CarteDetectAdv2009::ActiverEnvoiMesureAdvAuto)
	    .def("DesactiverEnvoiMesureAdvAuto", &CarteDetectAdv2009::DesactiverEnvoiMesureAdvAuto);

	class_<CarteServosNova2017>("CarteServosNova2017")
	    .def("EnvoyerPing", &CarteServosNova2017::EnvoyerPing)
	    .def("verifierPong", &CarteServosNova2017::verifierPong)
	    .def("ReglerPosition", &CarteServosNova2017::ReglerPosition)
	    .def("ActualiserPosition", &CarteServosNova2017::ActualiserPosition)
	    .def("verifierPositionActualisee", &CarteServosNova2017::verifierPositionActualisee)
	    .def("LirePosition", &CarteServosNova2017::LirePosition)
	    .def("verifierDeplacementTermine", &CarteServosNova2017::verifierDeplacementTermine)
	    .def("verifierServoBloque", &CarteServosNova2017::verifierServoBloque);

	class_<CarteLcd2009>("CarteLcd2009")
	    .def("EnvoyerPing", &CarteLcd2009::EnvoyerPing)
	    .def("verifierPong", &CarteLcd2009::verifierPong)
	    .def("getIA", &CarteLcd2009::getIA)
	    .def("EnvoyerIA", &CarteLcd2009::EnvoyerIA)
	    .def("getLastID", &CarteLcd2009::getLastID)
	    .def("ClearIA", &CarteLcd2009::ClearIA)
	    .def("AfficherTerminal", &CarteLcd2009::AfficherTerminal)
	    .def("AfficherPerso", &CarteLcd2009::AfficherPerso);

	class_<CarteContacteurs2007>("CarteContacteurs2007")
	    .def("EnvoyerPing", &CarteContacteurs2007::EnvoyerPing)
	    .def("verifierPong", &CarteContacteurs2007::verifierPong)
	    .def("ActualiserContacteurs", &CarteContacteurs2007::ActualiserContacteurs)
	    .def("SontContacteursActualises", &CarteContacteurs2007::SontContacteursActualises)
	    .def("getContacteur", &CarteContacteurs2007::getContacteur);

	class_<CarteCodesBarres2011>("CarteCodesBarres2011")
	    .def("EnvoyerPing", &CarteCodesBarres2011::EnvoyerPing)
	    .def("verifierPong", &CarteCodesBarres2011::verifierPong)
	    .def("getCodeLu", &CarteCodesBarres2011::getCodeLu);

	class_<CarteAlimentation2010>("CarteAlimentation2010")
	    .def("EnvoyerPing", &CarteAlimentation2010::EnvoyerPing)
	    .def("verifierPong", &CarteAlimentation2010::verifierPong)
	    .def("getTension", &CarteAlimentation2010::getTension);
}

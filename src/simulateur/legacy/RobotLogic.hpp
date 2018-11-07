// Robot.cpp

#include "../core/Simulateur.h"
#include "communication/Communicateur.h"
#include "communication/CommunicateurPipe.h"
#include "communication/CommunicateurSerie.h"
#include "communication/CommunicateurTCPIP.h"

#include <cstdio>

#define DESACTIVER_ADVERSAIRE

inline bool RobotLogic::onRecu(Trame const& trame, bool isAck) {
	if(this->_cartes[trame.getId()] == nullptr)
		return false;

	this->_cartes[trame.getId()]->onRecu(trame, isAck);

	return true;
}

// lors de l'émission d'une trame la redistribuer vers le communicateur
inline void RobotLogic::onEnvoye(Trame const& t, bool isAck) {
	// attention si le robot est sans port de comunication le communicateur est null
	if(_communicateur)
		this->_communicateur->onEnvoye(t, isAck);
}


// L'argument 'connexion' est soit "", soit de la forme "RS232:/dev/ttyS0" ou "TCPIP:1234" ou encore
// "PIPES:/tmp/read.pipe:/tmp/write.pipe"
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::RobotLogicTemplate(std::unique_ptr<Commun::ConstantesCommunes> constantesCommunes,
                                                                              std::unique_ptr<Commun::ConstantesRobot> constantes,
                                                                              _CouleurEquipe couleur,
                                                                              const std::string& connexion)
        : RobotLogic(std::move(constantesCommunes), std::move(constantes)), _couleur(couleur) {
	// attacher le bon communicateur au robot
	if(connexion != "" && connexion.length() >= 5) {
		std::string type = connexion.substr(0, 5);
		if(type.compare("RS232") == 0) {
			_communicateur = std::make_unique<Serie>(connexion.substr(6, connexion.length() - 6), *this);
		} else if(type.compare("TCPIP") == 0) {
			_communicateur = std::make_unique<TCPIP>(Utils::strToNumber<uint16_t>(connexion.substr(6)), *this);
		} else if(type.compare("LOCAL") == 0) {
			_communicateur = std::make_unique<TCPIP>(this->constantes().getPortTCPIPDefault(), *this);
		} else if(type.compare("PIPES") == 0) {
			std::string args = connexion.substr(6);
			std::string path_read = args.substr(0, args.find_first_of(":"));
			std::string path_write = args.substr(args.find_first_of(":") + 1);
			_communicateur = std::make_unique<NamedPipe>(path_read, path_write, *this);
		} else if(type.compare("conso") == 0) {
			// _communicateur = std::make_unique<Console>(*this);
		} else
			throw Communicateur::ErreurOuvertureCommunication("Port de communcation '"s + type + "' non reconnu !"s);
	} else if(connexion != "")
		throw Communicateur::ErreurOuvertureCommunication("Port de communcation '"s + connexion + "' non reconnu !"s);
	else
		logInfo("creation d'un robot sans port de communication");
}

// destructeur
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::~RobotLogicTemplate() {
	// NB : on est obligés d'appeler directement deconnecter(), car il est impossible de faire ça
	// dans le destructeur de communicateur à cause du thread de réception.
	if(_communicateur)
		_communicateur->cleanUp();
}

// ajouter une carte au robot
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
void RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::add(std::unique_ptr<Carte> carte) {
	// lève ErreurCarteDejaAjoute si une carte de même id est deja présente dans le robot
	if(carte->getId() >= Trame::NB_CARTES_MAX)
		throw ErreurIDCarteTropGrand(carte->getId());
	if(_cartes[carte->getId()])
		throw ErreurCarteDejaAjoutee(carte->getId());
	_cartes[carte->getId()] = std::move(carte);
}


// met à jour le robot en mettant à jour les cartes
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
void RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::update(Duration elapsed) {
	// si le robot est connecte à l'ia
	if(_communicateur && _communicateur->estConnecte()) {
		// mettre à jour les cartes et traiter le buffer de réception
		for(int numCarte = 0; numCarte < Trame::NB_CARTES_MAX; numCarte++) {
			if(_cartes[numCarte]) {
				_cartes[numCarte]->update(elapsed);
			}
		}
	}
}

/*
// Implémentation de l'interface ICoordonneesAdv
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
const Vector2m &RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::getPositionAdversaire() const {
#ifdef DESACTIVER_ADVERSAIRE
    static Vector2m const posAdversaire(100_m, 100_m);

    return posAdversaire;
#else
    auto &monde = static_cast<typename _Robot::_Monde const &>(Simulateur::getInstance().getMonde());
    return monde.getRobotAdverse(*this).getPosition();
#endif
}

// Implémentation de l'interface ICoordonneesAdv
template <typename _IDCartes, template <_IDCartes> class _CarteInfo, typename _CouleurEquipe>
Angle RobotLogicTemplate<_IDCartes, _CarteInfo, _CouleurEquipe>::getAngle2dAdversaire() const {
#ifdef DESACTIVER_ADVERSAIRE
    static Angle const angleAdversaire(0_deg);
    return angleAdversaire;
#else
    //auto &monde = static_cast<typename _Robot::_Monde const &>(Simulateur::getInstance().getMonde());
    //return monde.getRobotAdverse(*this).getPosition();
#endif

}*/

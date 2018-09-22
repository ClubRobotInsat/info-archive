/************************************************************
 * Strategie.cpp *
 * Classe de stratégie de match à durée limitée *
 * Version 0.1 - Guillaume Delamare 15/11/08 *
 ************************************************************/

#include "Strategie.h"
#include "ConstantesCommunes.h"
#include "RobotPrincipal/Constantes.h"
#include "sockets/tcpSock.h"
//#include "../Cartes/CarteDeplacement2009.h"

#include <Json.h>

#define NO_CARE_OF_ENV

namespace Commun {

	Strategie::Strategie(std::unique_ptr<Robot> robot, Constantes::RobotColor color) : _robot(std::move(robot)) {
		setThreadName("Main");
		// Création du robot, l'environnement est créé après définition du sens
		_robot->setStrategie(*this);

		// logDebug("Initialisation de l'environnement (NEW)");
		// Création de l'environnement.
		Distance sx = _robot->constantes().getRobotSize().x;
		Distance sy = _robot->constantes().getRobotSize().z;
		_env = std::make_unique<Environment>(_robot->constantesCommunes().getTailleGrille(),
		                                     0.01_m,
		                                     std::min(sx, sy) * 1.15,
		                                     (sqrt(sx * sx + sy * sy) / 2) * 1.35,
		                                     Vector2m{0_m, 0_m});

		//_robot->setTailleGrille(_robot->constantesCommunes().getTailleGrille());
		// Création du déplacement.
		// logDebug("Initialisation du DéplacementManager");
		_evitement = std::make_unique<Evitement>(*_robot, *_env);
		_dep = std::make_unique<Deplacement>(*_robot, *_env, *_evitement);

		/// Détermination de la couleur de l'équipe grâce à la carte contacteurs
		// Actualisation des contacteurs
		// logInfo("Je regarde la couleur");

		/*		// ID = 2 correspond au choix de l'équipe sur la carte IO
		#ifndef TARGET_SIMULATEUR
		        if(Simulateur::getInstance()->getRobotColor())
		#endif
		*/
		_couleurEquipe = color;

		// Création de l'environnement (pour l'A*).
		this->creerEnvironement();
		//_comLidar = std::thread([this]() { this->threadLidar(); });
	}

	/// Démarre la stratégie pour un match d'une durée donnée (en millisecondes)
	void Strategie::demarrer(Duration duree) {
		_dureeTotaleMatch = duree;
		logDebug9("Couleur du robot : ", this->getCouleurEquipe());
		logDebug("Debut du chrono match !");
		_chronoMatch.reset();

		_execution = std::thread(std::bind(&Strategie::exec, this));

		while(getTempsRestant() > 0_s)
			sleep(100_ms);

		logDebug("FIN DU MATCH !");
		// this->arreter();
		logDebug("Robot arrêté !");

		logDebug("Funny action !");
		this->funnyAction();

		auto handle = _execution.native_handle();
		_execution.detach();
		pthread_cancel(handle);
		pthread_join(handle, nullptr);
	}

	void Strategie::exec() {
		setThreadName("Strategie");
		// Autoriser l'interruption du thread à la volée
		// Pas propre (mélange std::thread et pthread, mais surtout pas de libération des ressources (locks,
		// allocations…)
		// !!!)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

		this->executer();
	}

	void Strategie::funnyAction() {}

	Duration Strategie::getTempsRestant() const {
		sleep(1000_ms);
		// logDebug6("Il reste :", (_dureeTotaleMatch - _chronoMatch.getElapsedTime()).toS());
		return _dureeTotaleMatch - _chronoMatch.getElapsedTime();
	}

	Duration Strategie::getTempsDepuisDebut() const {
		return _chronoMatch.getElapsedTime();
	}

	void Strategie::resetTimer() {
		_chronoMatch.reset();
	}

	void Strategie::arreter() {
		_robot->desactivation();
	}

	void Strategie::attendreTirette() {
		_robot->wait_for_tirette();
	}

	void Strategie::threadLidar() {
		while(true) {
			try {
				TcpSock sock("localhost", 5000);
				sock.write("i" + std::to_string(_robot->constantes().getIDRobot()) + '\n');

				int const firstDynamic = 10;
				int lastDynamicCount = 0;

				while(true) {
					auto posRobot = _robot->read_coordinates().getPos2D();
					std::unique_lock<std::mutex> lk(_robotsMutex);
					_tousLesRobots.clear();
					for(int i = firstDynamic; i != firstDynamic + lastDynamicCount; ++i) {
						this->_env->removeDynamicShape(i);
					}
					{
						sock.write("ps " + std::to_string(posRobot.x.template toMm<int>()) + ' ' +
						           std::to_string(posRobot.y.template toMm<int>()) + '\n');
					}

					{
						sock.write("r\n");
						std::istringstream adv(sock.readTextTo('\n'));
						int advCount;
						adv >> advCount;

						std::vector<Vector2m> advPos;
						advPos.reserve(advCount);

						for(int i = 0; i < advCount; ++i) {
							int v;
							Vector2m pos;
							adv >> v;
							pos.x = Distance::makeFromMm(v);
							adv >> v;
							pos.y = 2_m - Distance::makeFromMm(v);
							advPos.push_back(pos);
							_tousLesRobots.emplace_back(pos, false);
						}

						std::sort(advPos.begin(), advPos.end(), [posRobot](auto const& pos1, auto const& pos2) {
							return (pos1 - posRobot).squaredNorm() > (pos2 - posRobot).squaredNorm();
						});


						for(size_t i = 0; i + 1 < advPos.size(); ++i) {
							this->_env->addDynamicShape(firstDynamic + lastDynamicCount++,
							                            std::make_unique<Circle>(Environment::DANGER_INFINITY, 20_cm, advPos[i]));
							// logInfo("Robot ennemi détecté en position ", pos);
						}
					}

					{
						sock.write("pg\n");
						std::istringstream robots(sock.readTextTo('\n'));
						int robotsCount;
						robots >> robotsCount;

						for(int i = 0; i < robotsCount; ++i) {
							int v;
							robots >> v;
							if(v != _robot->constantes().getIDRobot()) {
								Vector2m pos;
								int id = 0;
								robots >> id;
								pos.x = Distance::makeFromMm(v);
								robots >> v;
								pos.y = 2_m - Distance::makeFromMm(v);
								this->_env->addDynamicShape(firstDynamic + lastDynamicCount++,
								                            std::make_unique<Circle>(Environment::DANGER_INFINITY, 20_cm, pos));
								// logInfo("Robot ami ", id, " détecté en position ", pos);
								_tousLesRobots.emplace_back(pos, true);
							}
						}
					}

					lk.unlock();

					sleep(0.2_s);
				}
			} catch(...) {
				sleep(4_s);
				continue;
			}
		}
	}

	/// Pointeurs vers l'environnement et le meca manager de l'année adéquate
	// std::unique_ptr<Environnement2015> _env;
	void Strategie::creerEnvironement() {
		this->_env->loadFromJSON(Constantes::TABLE_2018);
		// this->_env->loadFromFile("../../table_2018.json");
	}

	/**
	 * @return Une liste de paires <position, jeSuisAmi>. Le booléen à true veut dire que c'est un robot ami.
	 */
	std::list<std::pair<Vector2m, bool>> Strategie::tousLesRobots() {
		std::lock_guard<std::mutex> lk(_robotsMutex);
		return _tousLesRobots;
	}
} // namespace Commun
//
// Created by terae on 06/10/18.
//

#include <log/Log.h>

#include "AbstractStrategy.h"

#include "EmbeddedFiles.h"

namespace Strategy {
	AbstractStrategy::AbstractStrategy(Constants::RobotColor color) : _color(color), _nb_points(0) {
		setThreadName("Main");

		Distance sx = GLOBAL_CONSTANTS()["primary"].get_size().x;
		Distance sy = GLOBAL_CONSTANTS()["primary"].get_size().y;
		_env = std::make_unique<Environment>(GLOBAL_CONSTANTS().get_table_env_grid(),
		                                     GLOBAL_CONSTANTS().get_scale_environment(),
		                                     std::min(sx, sy) * 1.15,
		                                     (sqrt(sx * sx + sy * sy) / 2) * 1.35,
		                                     Vector2m{0_m, 0_m});

		this->create_environment();
	}

	void AbstractStrategy::create_environment() {
		this->_env->loadFromJSON(GLOBAL_CONSTANTS().TABLE_2018());
	}

	void AbstractStrategy::start(Duration match) {
		_total_duration_match = match;

		logDebug9("Couleur du robot : ", this->get_color());
		logDebug("Début du match !");
		this->reset_timer();

		_execution = std::thread(std::bind(&AbstractStrategy::exec, this));

		while(get_left_time() > 0_s) {
			sleep(1_s);
		}

		stop();

		logDebug("FIN DU MATCH !");
		logDebug("Funny action");
		this->funny_action();

		// Arrêt du thread d'exécution
		auto handle = _execution.native_handle();
		_execution.detach();
		pthread_cancel(handle);
		pthread_join(handle, nullptr);

		stop();
	}

	void AbstractStrategy::stop() {
		for(auto interfacer : _interfacers) {
			interfacer->get_robot()->deactivation();
		}
	}

	Duration AbstractStrategy::get_left_time() const {
		// logDebug6("Il reste :", (_dureeTotaleMatch - _chronoMatch.getElapsedTime()).toS());
		return _total_duration_match - get_time();
	}

	Duration AbstractStrategy::get_time() const {
		return _chrono_match.getElapsedTime();
	}

	void AbstractStrategy::reset_timer() {
		_chrono_match.reset();
	}

	std::shared_ptr<Interfacer::GlobalManager> AbstractStrategy::add_robot(std::unique_ptr<PhysicalRobot::Robot> robot) {
		auto manager = std::make_shared<Interfacer::GlobalManager>(std::move(robot));
		_interfacers.push_back(manager);
		return manager;
	}

	std::shared_ptr<Interfacer::GlobalManager> AbstractStrategy::add_robot(std::shared_ptr<Interfacer::GlobalManager> manager) {
		_interfacers.push_back(manager);
		return manager;
	}

	int AbstractStrategy::get_points() const {
		return _nb_points;
	}

	int AbstractStrategy::add_points(int n) {
		return _nb_points += n;
	}

	void AbstractStrategy::exec() {
		setThreadName("Strategy");
		// Autoriser l'interruption du thread à la volée
		// Pas propre (mélange std::thread et pthread, mais surtout pas de libération des ressources (locks,
		// allocations…)
		// !!!)
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);

		this->execute();
	}
} // namespace Strategy

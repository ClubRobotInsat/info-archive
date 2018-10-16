//
// Created by terae on 06/10/18.
//

#include <log/Log.h>

#include "AbstractStrategy.h"

#include "EmbeddedFiles.h"

namespace Strategy {
	AbstractStrategy::AbstractStrategy(std::unique_ptr<PhysicalRobot::Robot> robot, Constants::RobotColor color)
	        : _robot(std::move(robot)), _color(color) {
		setThreadName("Main");

		Distance sx = GLOBAL_CONSTANTS["primary"].get_size().x;
		Distance sy = GLOBAL_CONSTANTS["primary"].get_size().y;
		_env = std::make_unique<Environment>(GLOBAL_CONSTANTS.get_table_env_grid(),
		                                     GLOBAL_CONSTANTS.get_scale_environment(),
		                                     std::min(sx, sy) * 1.15,
		                                     (sqrt(sx * sx + sy * sy) / 2) * 1.35,
		                                     Vector2m{0_m, 0_m});

		this->create_environment();
	}

	void AbstractStrategy::create_environment() {
		this->_env->loadFromJSON(nlohmann::json::parse(EmbeddedFiles::readText("table.json")));
	}

	void AbstractStrategy::start(Duration match) {
		_total_duration_match = match;

		logDebug9("Couleur du robot : ", this->get_color());
		logDebug("Début du match !");
		this->reset_timer();

		_execution = std::thread(std::bind(&AbstractStrategy::exec, this));

		while(get_left_time() > 0_s)
			sleep(100_ms);

		logDebug("FIN DU MATCH !");
		logDebug("Funny action");
		this->funny_action();

		// Arrêt du thread d'exécution
		auto handle = _execution.native_handle();
		_execution.detach();
		pthread_cancel(handle);
		pthread_join(handle, nullptr);
	}

	Duration AbstractStrategy::get_left_time() const {
		sleep(1000_ms);
		// logDebug6("Il reste :", (_dureeTotaleMatch - _chronoMatch.getElapsedTime()).toS());
		return _total_duration_match - get_time();
	}

	Duration AbstractStrategy::get_time() const {
		return _chrono_match.getElapsedTime();
	}

	void AbstractStrategy::reset_timer() {
		_chrono_match.reset();
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

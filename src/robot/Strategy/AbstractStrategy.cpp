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

		Distance sx = GLOBAL_CONSTANTS()["primary"].get_size().x;
		Distance sy = GLOBAL_CONSTANTS()["primary"].get_size().y;
		_env = std::make_unique<Environment>(GLOBAL_CONSTANTS().get_table_env_grid(),
		                                     GLOBAL_CONSTANTS().get_scale_environment(),
		                                     std::min(sx, sy) * 1.15,
		                                     (sqrt(sx * sx + sy * sy) / 2) * 1.35,
		                                     Vector2m{0_m, 0_m});

		this->create_environment();

		auto adversary_finder = [this]() {
			Lidar::OccupGrid lidar_map(toVec2(GLOBAL_CONSTANTS().get_table_size()), 100, 66);

			while(get_left_time() > 0_s) {
				repere::Coordinates coords; // = this->_robot->get_module<PhysicalRobot::Moving>()::get_coordinates();
				lidar_map.reset();

				auto frame = _robot->get_lidar_frame();
				if(frame != std::nullopt) {
					lidar_map.accumulate(frame.value(), coords);
					Lidar::FindRobots robots;
					robots.accumulate(lidar_map);
					_mutex_adversary.lock();
					_adversary_positions = robots.get_results();
				} else {
					_mutex_adversary.lock();
					_adversary_positions.clear();
				}
				_mutex_adversary.unlock();
			}
		};
		_find_robots = std::thread(adversary_finder);
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

		_find_robots.join();
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

	std::vector<repere::Position> AbstractStrategy::get_adversary_positions() const {
		std::lock_guard<std::mutex> lk(_mutex_adversary);
		return _adversary_positions;
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

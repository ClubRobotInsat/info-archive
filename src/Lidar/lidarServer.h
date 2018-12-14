#pragma once
/******************************************************************************/
/* info/robot/Lidar/lidarServerFront.cpp                           2015-05-17 */
/* Club Robot INSA Toulouse                                      Félix Poisot */
/******************************************************************************/
#include "Driver/lidar.h"
#include "FindRobots.h"
#include "affiche.h"
#include "filtre.h"
#include "lidarsToGrid.h"
#include <mutex>
#include <thread>
/******************************************************************************/

// Regroupe les Lidars, avec un thread pour les lires / reconnecter
// sortie graphique en option.
class LidarThread {
public:
	LidarThread(bool useGL);
	~LidarThread() {
		_thread->join();
	}

	void start() {
		_thread = std::make_unique<std::thread>([this]() { run(); });
	}
	void run();

	// Dis si les lidars sont branchés et fonctionnels
	// {Sick, Hokuyo}
	std::vector<bool> status() const;

	std::vector<Vec2> releves();
	// Pré-concaténés pour la performance de TCP.
	std::string relevesAsText();
	// pour l'ajustement en début de match.
	std::string relevesChrMap();

private:
	std::unique_ptr<Lidar> _sick;
	std::unique_ptr<Lidar> _hokuyo;
	FindRobots _tr;
	std::mutex _lTr;
	std::unique_ptr<std::thread> _thread;
	bool _hasGL;
	std::unique_ptr<Affiche> _aff; // optionnel.
};
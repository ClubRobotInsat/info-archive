//
//  display.cpp
//  Club Robot
//
//  Created by Rémi on 10/03/2015.
//

#include "affiche.h"
#include "filtre.h"
#include "trouverobots.h"
#include <log/Log.h>

using Vec2 = Vector2f;
using Vec3 = Vector3f;
using Vec4 = Vector4f;

const double MAP_W = 3; // mètres
const double MAP_H = 2; //

const int RESX = 100;
const int RESY = 66;


std::unique_ptr<Lidar> source1, source2;
std::unique_ptr<Filtre> filtre1, filtre2;
std::unique_ptr<OccupGrid> map;
TrouveRobots robots;
std::unique_ptr<Affiche> affiche;

int savedFrames = 0;
int savedCount = 10;


void refresh() {
	// if(savedFrames < savedCount)
	//	mesure.saveToFile("/tmp" + std::to_string(savedFrames++));

	map->reset();

	if(source1) {
		Vec2 pos = {0, 1};
		auto mesure = source1->getTrame();
		auto mf = filtre1->getTrame(mesure);
		affiche->trameLidar(mf, pos, 0_deg, {0, 0.5, 0, 0.7f});
		map->accumulate(mf, pos, 0_deg);
	}
	if(source2) {
		Vec2 pos = {3, 2};
		auto mesure = source2->getTrame();
		auto mf = filtre2->getTrame(mesure);
		affiche->trameLidar(mf, pos, 180_deg, {0, 0.5, 0.5, 0.7f});
		map->accumulate(mf, pos, 180_deg);
	};

	affiche->grille(*map, {0, 0, 0});

	robots.accumulate(*map);
	affiche->candidats(robots.getResults(), {1, 0.5f, 0});
}

int main(int argc, char** argv) {
	Log::open(argc, argv, false);

	affiche = std::make_unique<Affiche>(Vec2(MAP_W, MAP_H));

	try {
		source1 = Lidar::openLidar(Lidar::Sick);
		filtre1 = std::make_unique<Filtre>();
	} catch(std::runtime_error& e) {
		logWarn("Impossible d'ouvrir lidar Sick");
	}

	try {
		source2 = Lidar::openLidar(Lidar::Hokuyo);
		filtre2 = std::make_unique<Filtre>();
	} catch(std::runtime_error& e) {
		logWarn("Impossible d'ouvrir lidar Hokuyo");
	}

	map = std::make_unique<OccupGrid>(MAP_W, MAP_H, RESX, RESY);

	while(!affiche->isClosed()) {
		affiche->begin();
		refresh();
		affiche->end();
	}

	return 0;
}

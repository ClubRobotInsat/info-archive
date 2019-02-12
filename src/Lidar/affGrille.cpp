//
//  display.cpp
//  Club Robot
//
//  Created by Rémi on 10/03/2015.
//

#include "Display.h"
#include "FindRobots.h"
#include "filtre.h"
#include <log/Log.h>

using Vec2 = Vector2f;
using Vec3 = Vector3f;
using Vec4 = Vector4f;

const int RESX = 100;
const int RESY = 66;

std::unique_ptr<Lidar> source1, source2;
std::unique_ptr<Filtre> filtre1, filtre2;
std::unique_ptr<OccupGrid> map;
FindRobots robots;
std::unique_ptr<Display> affiche;

int savedFrames = 0;
int savedCount = 10;


void refresh() {
	// if(savedFrames < savedCount)
	//	mesure.save_to_file("/tmp" + std::to_string(savedFrames++));

	map->reset();

	if(source1) {
		repere::Coordinates coords({0_m, 1_m}, 0_deg);
		auto mesure = source1->get_frame();
		auto mf = filtre1->get_frame(mesure);
		affiche->frame_lidar(mf, coords, {0, 0.5, 0, 0.7f});
		map->accumulate(mf, coords);
	}
	if(source2) {
		repere::Coordinates coords({3_m, 2_m}, 180_deg);
		auto mesure = source2->get_frame();
		auto mf = filtre2->get_frame(mesure);
		affiche->frame_lidar(mf, coords, {0, 0.5, 0.5, 0.7f});
		map->accumulate(mf, coords);
	};

	affiche->grid(*map, {0, 0, 0});

	robots.accumulate(*map);
	affiche->candidates(robots.get_results(), {1, 0.5f, 0});
}

int main(int argc, char** argv) {
	Log::open(argc, argv, false);

	affiche = std::make_unique<Display>(toVec2(GLOBAL_CONSTANTS().get_table_size()));

	try {
		source1 = Lidar::open_lidar(Lidar::Sick);
		filtre1 = std::make_unique<Filtre>();
	} catch(std::runtime_error& e) {
		logWarn(e.what());
		logWarn("Impossible d'ouvrir lidar Sick");
	}

	try {
		source2 = Lidar::open_lidar(Lidar::Hokuyo);
		filtre2 = std::make_unique<Filtre>();
	} catch(std::runtime_error& e) {
		logWarn("Impossible d'ouvrir lidar Hokuyo");
	}

	map = std::make_unique<OccupGrid>(toVec2(GLOBAL_CONSTANTS().get_table_size()), RESX, RESY);

	while(!affiche->isClosed()) {
		affiche->begin();
		refresh();
		affiche->end();
	}

	return 0;
}

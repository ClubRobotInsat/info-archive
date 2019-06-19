//
// Created by terae on 05/01/19.
//

#include "FindRobots.h"
#include "filtre.h"
#include <Constants.h>

template <typename T>
T get_input(std::string msg) {
	std::cout << msg << "? " << std::flush;
	T result;
	std::cin >> result;
	return result;
}

int main() {
	std::unique_ptr<Lidar> lidar = Lidar::Lidar::open_lidar(Lidar::Sick);
	if(lidar == nullptr) {
		std::cerr << "No lidar found, exiting" << std::endl;
		return 5;
	}

	std::unique_ptr<Filtre> filtre = std::make_unique<Filtre>();
	std::unique_ptr<OccupGrid> map = std::make_unique<OccupGrid>(toVec2(GLOBAL_CONSTANTS().get_table_size()), 100, 66);
	FindRobots robots;

	Constants::RobotColor color;
	auto str = get_input<std::string>("color ([y|yellow|p|purple]");
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	if(str == "y" || str == "yellow") {
		color = Constants::RobotColor::Yellow;
	} else {
		color = Constants::RobotColor::Purple;
	}

	Distance x = Distance::makeFromCm(get_input<double>("pos.x (cm)"));
	Distance y = Distance::makeFromCm(get_input<double>("pos.y (cm)"));
	Angle angle = Angle::makeFromDeg(get_input<double>("angle (°)"));
	while(true) {
		map->reset();
		repere::Coordinates coords({x, y}, angle, GLOBAL_CONSTANTS().get_reference(color));
		auto record = lidar->get_frame();
		auto record_filtered = filtre->get_frame(record);
		map->accumulate(record_filtered, coords);

		robots.accumulate(*map);
		int i = 0;
		for(repere::Position pos : robots.get_results()) {
			std::cout << "robot " << i++ << " found: " << pos.getPos2D() << " (absolute reference)" << std::endl;
		}
		std::cout << std::endl;
	}
}
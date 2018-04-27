//
// Created by paul on 24/04/18.
//

#include <iostream>
#include <thread>

int main_elec();
int main_robot(int argc, char** argv);

int main_robot_2() {
	char* args[2];
	args[0] = const_cast<char*>("./toto");
	args[1] = const_cast<char*>("NULL");

	return main_robot(2, args);
}

int main() {
	std::cout << "Début de l'elec" << std::endl;
	std::thread thread_elec(&main_elec);
	std::cout << "Début de l'info " << std::endl;
	std::thread thread_robot(&main_robot_2);
	thread_elec.join();
	thread_robot.join();
	return 0;
}
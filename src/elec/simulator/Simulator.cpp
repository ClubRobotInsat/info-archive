//
// Created by paul on 24/04/18.
//

#include <iostream>
#include <thread>

int main_elec();
int main_robot(int argc, char** argv);

int main_robot_2() {
	char* arg1 = "RIEN";
	char* arg2 = "DU TOUT";
	char* args[2];
	args[1] = arg1;
	args[2] = arg2;
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
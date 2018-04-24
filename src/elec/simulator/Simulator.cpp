//
// Created by paul on 24/04/18.
//

#include <thread>

int main_elec();
int main_robot();

int main() {
	std::thread thread_elec(&main_elec);
	std::thread thread_robot(&main_robot);
	thread_elec.join();
	thread_robot.join();
}
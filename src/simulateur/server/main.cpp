#include <csignal>
#include <functional>
#include <thread>


#include "SimuWebSocketServer.h"

std::function<void()> exitHandler;

int main() {
	SimuWebSocketServer server{5000};
	server.start();

	exitHandler = [&]() { server.stop(); };

	Message m;
	m.add("hey", 2);
	MessageBatch list;
	list.add(m);
	list.add(m);
	list.add(m);
	std::cout << list.getAndClear() << std::endl;
	bool ok = true;

	while(ok) {
		std::string rep;
		std::cin >> rep;
		if(rep == "stop") {
			ok = false;
			server.stop();
		} else {
			server.broadcast(rep);
		}
	}

	std::signal(SIGTERM, [](int) { exitHandler(); });
	std::signal(SIGINT, [](int) { exitHandler(); });
	return EXIT_SUCCESS;
}

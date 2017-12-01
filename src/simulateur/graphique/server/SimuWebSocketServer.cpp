#include "SimuWebSocketServer.h"
#include "../../core/Simulateur.h"

SimuWebSocketServer::SimuWebSocketServer(const int port) : _port(port) {
	_clientConnected = false;
	_server.set_open_handler(std::bind(&SimuWebSocketServer::onConnect, this, std::placeholders::_1));
	_server.set_close_handler(std::bind(&SimuWebSocketServer::onDisconnect, this, std::placeholders::_1));
	_server.set_message_handler(std::bind(&SimuWebSocketServer::onMessage, this, std::placeholders::_1, std::placeholders::_2));
}

SimuWebSocketServer::~SimuWebSocketServer() {
	_server.stop();
}

void SimuWebSocketServer::addServerListener(IServerListener* listener) {
	_listeners.push_back(listener);
}

void SimuWebSocketServer::onMessage(SimuWebSocketServer::Client, SimuWebSocketServer::Message message) {
	std::cout << message->get_payload() << std::endl;
}

void SimuWebSocketServer::onConnect(SimuWebSocketServer::Client client) {
	sleep(1);
	_clientConnected = true;
	_clients.insert(client);
	for(auto listener : _listeners)
		listener->onConnect(client);
	std::cout << "Client connected." << std::endl;
}

void SimuWebSocketServer::onDisconnect(SimuWebSocketServer::Client client) {
	_clientConnected = false;
	_clients.erase(client);
}

void SimuWebSocketServer::start() {
	std::cout << "SimuWebSocketServer running on ws://localhost:" << _port << " ..." << std::endl;
	_server.clear_access_channels(websocketpp::log::alevel::all);

	_server.set_reuse_addr(true);

	_server.init_asio();
	_server.listen(_port);
	_server.start_accept();
	_thread = std::thread(std::bind(&Server::run, &_server));
}

void SimuWebSocketServer::stop() {
	std::cout << "SimuWebSocketServer stopping ..." << std::endl;
	//_server.stop_listening();
	_server.stop();
	_thread.join();
	std::cout << "SimuWebSocketServer stopped !" << std::endl;
}

void SimuWebSocketServer::broadcast(const std::string& message) {
	// std::cout << "Broadcast : " << message << std::endl;
	for(auto& client : _clients) {
		_server.send(client, message, websocketpp::frame::opcode::text);
	}
}

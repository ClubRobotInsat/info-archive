#pragma once

#define ASIO_STANDALONE

#include <set>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class SimuWebSocketServer {
public:
	using Server = websocketpp::server<websocketpp::config::asio>;
	using Client = websocketpp::connection_hdl;
	using Message = Server::message_ptr;

	SimuWebSocketServer(const int port = 5000);
	~SimuWebSocketServer();

	void onMessage(Client client, Message message);
	void onConnect(Client client);
	void onDisconnect(Client client);

	void broadcast(const std::string& message);

	void start();
	void stop();

	bool isClientConnected() {
		return _clientConnected;
	};

private:
	int _port;
	Server _server;
	std::set<Client, std::owner_less<Client>> _clients;
	std::thread _thread;
	bool _clientConnected = false;
};

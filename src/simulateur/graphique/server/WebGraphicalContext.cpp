#include "WebGraphicalContext.h"

#include "../IGraphicalInstance.h"
#include "SimulationToWebserver.h"
#include "WebObjectHandler.h"

WebGraphicalContext::WebGraphicalContext(int port) : _server(port), _maxId(-1) {
	_server.addServerListener(this);
	_server.start();
}

WebGraphicalContext::~WebGraphicalContext() {
	_server.stop();
}

void WebGraphicalContext::update() {
	JSON& list = _messageBuf;

	// Actualisation des données des objets.
	for(auto& object : _objects) {
		if(object->_created) {
			list.push_back(object->getCreationMessage());
			object->_created = false;
		} else if(object->_changed) {
			list.push_back(object->getUpdateMessage());
			object->_changed = false;
		}
	}

	// Envoi
	std::stringstream ss;
	ss << list;
	std::string s = ss.str();

	_server.broadcast(s);

	// Réinitialisation du buffer de messages
	list.clear();
}

void WebGraphicalContext::displayMessage(std::string msg) {
	JSON message;
	message["type"] = "log";
	message["message"] = msg;
	_messageBuf.push_back(message);
}

IGraphicalInstance* WebGraphicalContext::createDefaultObject() {
	return addObject(*this, nextId());
}

IGraphicalInstance* WebGraphicalContext::createCylinder(const Vector3m& position, Length radius, Length height) {
	WebObjectHandler* object = addObject(*this, nextId());

	JSON content;
	content["type"] = "newcylinder";
	content["radius"] = toWebServer(radius);
	content["height"] = toWebServer(height);
	object->setAdditionalContent(content);

	return object;
}

IGraphicalInstance* WebGraphicalContext::createSphere(const Vector3m& position, Length radius) {
	WebObjectHandler* object = addObject(*this, nextId());

	JSON content;
	content["type"] = "newsphere";
	content["radius"] = toWebServer(radius);
	object->setAdditionalContent(content);

	return object;
}

IGraphicalInstance* WebGraphicalContext::createCuboid(const Vector3m& position, const Vector3m& dimensions) {
	WebObjectHandler* object = addObject(*this, nextId());

	JSON content;
	content["type"] = "newcuboid";
	object->setAdditionalContent(content);

	object->setScale(toWebServer(dimensions));

	return object;
}

IGraphicalInstance* WebGraphicalContext::createModel(const Vector3m& position, const std::string& model) {
	WebObjectHandler* object = addObject(*this, nextId());

	JSON content;
	content["type"] = "newmodel";
	content["modelName"] = model;
	object->setAdditionalContent(content);

	return object;
}

void WebGraphicalContext::remove(IGraphicalInstance* object) {
	for(auto it = _objects.begin(); it == _objects.end(); it++) {
		if(object->getId() == (*it)->getId()) {
			_messageBuf.push_back((*it)->getRemoveMessage());
			_objects.erase(it);
			return;
		}
	}
}

int WebGraphicalContext::nextId() {
	_maxId++;
	return _maxId;
}

void WebGraphicalContext::onConnect(IServerListener::Client client) {
	JSON allObjects;

	for(auto& object : _objects) {
		allObjects.push_back(object->getCreationMessage());
	}

	std::stringstream ss;
	ss << allObjects;
	std::string s = ss.str();
	_server.sendToClient(client, s);
}

void WebGraphicalContext::onDisconnect(IServerListener::Client) {}

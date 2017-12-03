#include "WebGraphicalContext.h"

#include "../core/SimulationToWebserver.h"
#include "IGraphicalInstance.h"
#include "WebObjectHandler.h"

WebGraphicalContext::WebGraphicalContext(int port) : _server(port), _maxId(-1) {
	_server.addServerListener(this);
	_server.start();
}

WebGraphicalContext::~WebGraphicalContext() {
	_server.stop();
}

void WebGraphicalContext::update() {
	// Actualisation des données des objets.
	JSON list;

	for(auto& object : _objects) {
		if(object->_created) {
			list.append(object->getCreationMessage());
			object->_created = false;
		} else if(object->_changed) {
			list.append(object->getUpdateMessage());
			object->_changed = false;
		}
	}

	Json::FastWriter writer;
	writer.omitEndingLineFeed();

	std::string s = writer.write(list);
	_server.broadcast(s);
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
			// TODO [Important] broadcast delete message
			_objects.erase(it);
			return;
		}
	}
}

int WebGraphicalContext::nextId() {
	_maxId++;
	return _maxId;
}

void WebGraphicalContext::onConnect(IServerListener::Client) {
	JSON allObjects;

	for(auto& object : _objects) {
		allObjects.append(object->getCreationMessage());
	}

	Json::FastWriter writer;
	writer.omitEndingLineFeed();

	std::string s = writer.write(allObjects);
	// FIXME broadcast est un peu problématique ici, car si plusieurs servers, on se retrouve avec des objets en double
	_server.broadcast(s);
}

void WebGraphicalContext::onDisconnect(IServerListener::Client) {}
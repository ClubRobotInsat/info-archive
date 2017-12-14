//
// Created by louis on 30/11/17.
//

#ifndef ROOT_WEBGRAPHICALCONTEXT_H
#define ROOT_WEBGRAPHICALCONTEXT_H

#include <vector>

#include "../IGraphicalContext.h"
#include "SimuWebSocketServer.h"
#include "WebObjectHandler.h"

class WebGraphicalContext : public IGraphicalContext, public IServerListener {
public:
	WebGraphicalContext(int port = 5000);
	~WebGraphicalContext();

	void update() override;
	void displayMessage(std::string message) override;

	IGraphicalInstance* createDefaultObject() override;

	IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) override;
	IGraphicalInstance* createSphere(const Vector3m& position, Length radius) override;
	IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) override;
	IGraphicalInstance* createModel(const Vector3m& position, const std::string& model) override;

	void remove(IGraphicalInstance* object) override;

	void onConnect(Client client) override;
	void onDisconnect(Client client) override;

private:
	SimuWebSocketServer _server;

	int _maxId;
	std::vector<std::unique_ptr<WebObjectHandler>> _objects;

	JSON _messageBuf;

	template <typename... Args>
	WebObjectHandler* addObject(WebGraphicalContext& context, Args... args) {
		_objects.push_back(std::make_unique<WebObjectHandler>(context, args...));
		return _objects.back().get();
	}

	/** retourne un id inutilisé pour un objet, et incrémente
	 * le compteur d'id. */
	int nextId();
};


#endif // ROOT_WEBGRAPHICALCONTEXT_H

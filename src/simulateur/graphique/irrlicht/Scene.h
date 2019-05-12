//
// Created by denis on 26/01/17.
//

#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H
#include <string>
#include <vector>

#include <irrlicht/irrlicht.h>

#include "../IGraphicalContext.h"
#include "../IGraphicalInstance.h"
#include "../IGraphicalUserListener.h"
#include "Object.h"

class Scene : public IGraphicalContext {
public:
	Scene();
	void AddCamera();
	void AddCameraMaya(irr::f32 rotatespeed, irr::f32 zoomspeed, irr::f32 translationspeed);
	void AddCube(float size, irr::core::vector3df position);

	void update() override;
	void displayMessage(std::string message) override;

	virtual IGraphicalInstance* createDefaultObject();
	virtual IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions);
	virtual IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height);
	virtual IGraphicalInstance* createSphere(const Vector3m& position, Length radius);
	virtual IGraphicalInstance* createModel(const Vector3m& position, const std::string& model);
	virtual void remove(IGraphicalInstance* object);

	void ChangePosition(irr::core::vector3df position, int id);
	void Rotation(irr::core::vector3df rotation, int id);
	// void AddTexture (std::string image) ;
	void PutCameraObjet();
	void Loop();

	void addWindowListener(IGraphicalUserListener* listener) {
		_listeners.push_back(listener);
	}

private:
	void incrementId();

	Object& getAt(int id);

	irr::video::IVideoDriver* _driver;
	irr::IrrlichtDevice* _device;
	irr::scene::ISceneManager* _scenemanager;
	std::vector<std::unique_ptr<Object>> _listeObjet;

	std::vector<IGraphicalUserListener*> _listeners;
	int _objectId;
};

#endif // PROJECT_SCENE_H

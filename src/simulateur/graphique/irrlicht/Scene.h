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

class Scene : public IGraphicalContext, public irr::IEventReceiver {
public:
	Scene();

	void AddCamera();
	void AddCameraMaya(irr::f32 rotatespeed, irr::f32 zoomspeed, irr::f32 translationspeed);

	/** Add a gizmo representing the three directions XYZ.  */
	void AddAxis();
	void AddCube(float size, irr::core::vector3df position);

	void update() override;
	void displayMessage(std::string message) override;

	IGraphicalInstance* createDefaultObject() override;
	IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) override;
	IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) override;
	IGraphicalInstance* createSphere(const Vector3m& position, Length radius) override;
	IGraphicalInstance* createModel(const Vector3m& position, const std::string& model) override;
	void remove(IGraphicalInstance* object) override;

	bool OnEvent(const irr::SEvent& event) override;

	// void AddTexture (std::string image) ;
	void Loop();

	void addWindowListener(IGraphicalUserListener* listener) {
		_listeners.push_back(listener);
	}

private:
	Object& getAt(int id);

	irr::video::IVideoDriver* _driver;
	irr::IrrlichtDevice* _device;
	irr::scene::ISceneManager* _scenemanager;
	irr::scene::ICameraSceneNode* _camera;
	irr::scene::ISceneCollisionManager* _collisions;

	std::vector<std::unique_ptr<Object>> _listeObjet;

	std::vector<IGraphicalUserListener*> _listeners;
	int _objectId;


	void incrementId();

	/** Make camera look at the object corresponding to the given id. */
	void cameraLookAt();

	/** Ajoute le node à la scene, définit sa position et retourne son id.
	 * Cette méthode peut également effectuer d'autres opérations de setup
	 * sur l'objet. */
	Object& setupAndAddObject(irr::scene::ISceneNode* node, const Vector3m& position);

	Object& setupAndAddMeshObject(irr::scene::IMeshSceneNode* node, const Vector3m& position);
};

#endif // PROJECT_SCENE_H

//
// Created by denis on 26/01/17.
//

#ifndef PROJECT_SCENE_H
#define PROJECT_SCENE_H
#include "Camera.h"
#include "Object.h"
#include <irrlicht/irrlicht.h>
#include <string>
#include <vector>


class Scene : public IGraphicalContext {

public:
	Scene();
	void AddCamera();
	void AddCameraMaya(irr::f32 rotatespeed, irr::f32 zoomspeed, irr::f32 translationspeed);
	void AddCube(float size, irr::core::vector3df position);

	virtual IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) = 0;
	virtual IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) = 0;
	virtual IGraphicalInstance* createSphere(const Vector3m& position, Length radius) = 0;

	void ChangePosition(irr::core::vector3df position, int id);
	void Rotation(irr::core::vector3df rotation, int id);
	// void AddTexture (std::string image) ;
	void PutCameraObjet();
	void Loop();


private:
	void incrementId();

	irr::video::IVideoDriver* _driver;
	irr::IrrlichtDevice* _device;
	irr::scene::ISceneManager* _scenemanager;
	std::vector<Object> _listeObjet;
	int _objectId;
};


#endif // PROJECT_SCENE_H

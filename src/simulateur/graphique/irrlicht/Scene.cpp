//
// Created by denis on 26/01/17.
//

#include "Scene.h"
#include "SimulationToIrrlicht.h"

Scene::Scene() : _objectId(0) {
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 32, false, true, false, 0);
	_scenemanager = _device->getSceneManager();
	_driver = _device->getVideoDriver();
};

void Scene::AddCube(float size, irr::core::vector3df position) {
	irr::scene::ISceneNode* cube = _scenemanager->addCubeSceneNode(size, 0, -1, position, irr::core::vector3df(0, 180, 0));
	cube->setID(_objectId);

	_listeObjet.push_back(Object(_objectId, cube));
	incrementId();
};


virtual Scene::IGraphicalInstance* createCuboid(const Vector3m& position, const Vector3m& dimensions) {
	irr::scene::IMeshSceneNode* cube = _scenemanager->addCubeSceneNode(SimulationToIrrlicht::ToIrrlicht(dimensions.x),
	                                                                   0,
	                                                                   -1,
	                                                                   SimulationToIrrlicht::VectorIrr(position),
	                                                                   irr::core::vector3df(0, 180, 0));
	cube->setID(_objectId);

	_listeObjet.push_back(Object(_objectId, cube));
	incrementId();

	irr::scene::IMesh* _cubeMesh = cube->getMesh();
	irr::scene::IMeshManipulator* Mcube = _scenemanager->getMeshManipulator();
	float My = SimulationToIrrlicht::ToIrrlicht(dimensions.y) / SimulationToIrrlicht::ToIrrlicht(dimensions.x);
	float Mz = SimulationToIrrlicht::ToIrrlicht(dimensions.z) / SimulationToIrrlicht::ToIrrlicht(dimensions.x);
	Mcube->scale(_cubeMesh, irr::core::vector3df(1, My, Mz));
};

virtual Scene::IGraphicalInstance* createCylinder(const Vector3m& position, Length radius, Length height) {
	const irr::scene::IGeometryCreator* cylindreGeo = _scenemanager->getGeometryCreator();
	irr::scene::IMesh* cylindreMesh = cylindreGeo->createCylinderMesh(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                  SimulationToIrrlicht::ToIrrlicht(length),
	                                                                  irr::u32(10),
	                                                                  irr::video::SColor(255, 0, 0, 255),
	                                                                  true,
	                                                                  0.0);


	auto _meshNodeTube = _scenemanager->addMeshSceneNode(cylindreMesh);
	_meshNodeTube->setID(_objectId);
	_listeObjet.push_back(Object(_objectId, _meshNodeTube));
	Scene::ChangePosition(SimulationToIrrlicht::VectorIrr(position), _objectId);
	incrementId();
};

virtual Scene::IGraphicalInstance* createSphere(const Vector3m& position, Length radius) {
	irr::scene::ISceneNode* sphere = _scenemanager->addSphereSceneNode(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                   32,
	                                                                   0,
	                                                                   -1,
	                                                                   SimulationToIrrlicht::VectorIrr(position),
	                                                                   irr::core::vector3df(0, 0, 0),
	                                                                   irr::core::vector3df(1.0f, 1.0f, 1.0));
	sphere->setID(_objectId);
	_listeObjet.push_back(Object(_objectId, sphere));
	incrementId();
};


void Scene::ChangePosition(irr::core::vector3df position, int id) {
	_listeObjet[id].getInternalPtr()->setPosition(position);
};

void Scene::Rotation(irr::core::vector3df rotation, int id) {
	_listeObjet[id].getInternalPtr()->setRotation(rotation);
};

void Scene::Loop() {
	while(_device->run()) {
		_driver->beginScene(true, true, irr::video::SColor(255, 255, 255, 255));
		_scenemanager->drawAll();
		_driver->endScene();
	}
};

void Scene::AddCamera() {
	_scenemanager->addCameraSceneNode(0, irr::core::vector3df(-5, -5, -5), irr::core::vector3df(5, 0, 0));
};

void Scene::AddCameraMaya(irr::f32 rotatespeed, irr::f32 zoomspeed, irr::f32 translationspeed) {
	_scenemanager->addCameraSceneNodeMaya(0, rotatespeed, zoomspeed, translationspeed);
};

void Scene::PutCameraObjet() {
	_scenemanager->getActiveCamera()->setTarget(_listeObjet.at(0).getInternalPtr()->getAbsolutePosition());
};

void Scene::incrementId() {

	_objectId++;
}

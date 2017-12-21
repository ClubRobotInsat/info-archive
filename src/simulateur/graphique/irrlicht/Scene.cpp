//
// Created by denis on 26/01/17.
//

#include "Scene.h"
#include "SimulationToIrrlicht.h"

// TODO (pas urgent) lancer irrlicht sur un thread à part

Scene::Scene() : _objectId(0) {
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 32, false, true, false, 0);
	_scenemanager = _device->getSceneManager();
	_driver = _device->getVideoDriver();

	// Camera
	AddCameraMaya(-1000.f, 200.f, 100.f);
	_scenemanager->getActiveCamera()->setTarget(irr::core::vector3df(15.f, 0.f, 10.f));

	// Lumière venant du haut
	irr::scene::ILightSceneNode* light1 = _scenemanager->addLightSceneNode();
	light1->setLightType(irr::video::ELT_DIRECTIONAL);
	light1->setRotation(irr::core::vector3df(90, 0, 0));
};

void Scene::AddCube(float size, irr::core::vector3df position) {
	irr::scene::ISceneNode* cube = _scenemanager->addCubeSceneNode(size, 0, -1, position, irr::core::vector3df(0, 180, 0));
	cube->setID(_objectId);

	_listeObjet.push_back(std::make_unique<Object>(_objectId, cube));
	incrementId();
};

void Scene::update() {

	if(_device->run()) {
		// Mise à jour du ratio de la fenetre (si l'utilisateur la redimensionne)
		auto screenSize = _driver->getScreenSize();
		_scenemanager->getActiveCamera()->setAspectRatio((float)screenSize.Width / screenSize.Height);

		// Dessin de la scene
		_driver->beginScene(true, true, irr::video::SColor(255, 200, 200, 200));
		_scenemanager->drawAll();
		_driver->endScene();
	} else {
		for(IGraphicalUserListener* listener : _listeners) {
			listener->onExit();
		}

		_device->closeDevice();
	}
}

void Scene::displayMessage(std::string message) {
	// TODO displayMessage
}

IGraphicalInstance* Scene::createDefaultObject() {
	auto node = _scenemanager->addEmptySceneNode(0, _objectId);
	incrementId();

	_listeObjet.push_back(std::make_unique<Object>(_objectId, node));

	return _listeObjet.back().get();
};

IGraphicalInstance* Scene::createCuboid(const Vector3m& position, const Vector3m& dimensions) {
	irr::scene::IMeshSceneNode* cube =
	    _scenemanager->addCubeSceneNode(1, 0, -1, SimulationToIrrlicht::VectorIrr(position), irr::core::vector3df(0, 180, 0));
	cube->setID(_objectId);

	_listeObjet.push_back(std::make_unique<Object>(_objectId, cube));
	incrementId();

	irr::scene::IMesh* _cubeMesh = cube->getMesh();
	irr::scene::IMeshManipulator* Mcube = _scenemanager->getMeshManipulator();
	Mcube->scale(_cubeMesh, SimulationToIrrlicht::VectorIrr(dimensions));

	return _listeObjet.back().get();
};

IGraphicalInstance* Scene::createCylinder(const Vector3m& position, Length radius, Length height) {
	const irr::scene::IGeometryCreator* cylindreGeo = _scenemanager->getGeometryCreator();
	irr::scene::IMesh* cylindreMesh = cylindreGeo->createCylinderMesh(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                  SimulationToIrrlicht::ToIrrlicht(height),
	                                                                  irr::u32(10),
	                                                                  irr::video::SColor(255, 0, 0, 255),
	                                                                  true,
	                                                                  0.0);

	auto _meshNodeTube = _scenemanager->addMeshSceneNode(cylindreMesh);
	_meshNodeTube->setID(_objectId);
	_listeObjet.push_back(std::make_unique<Object>(_objectId, _meshNodeTube));
	Scene::ChangePosition(SimulationToIrrlicht::VectorIrr(position), _objectId);
	incrementId();

	return _listeObjet.back().get();
};

IGraphicalInstance* Scene::createSphere(const Vector3m& position, Length radius) {
	irr::scene::ISceneNode* sphere = _scenemanager->addSphereSceneNode(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                   32,
	                                                                   0,
	                                                                   -1,
	                                                                   SimulationToIrrlicht::VectorIrr(position),
	                                                                   irr::core::vector3df(0, 0, 0),
	                                                                   irr::core::vector3df(1.0f, 1.0f, 1.0));

	sphere->setID(_objectId);
	_listeObjet.push_back(std::make_unique<Object>(_objectId, sphere));
	incrementId();

	return _listeObjet.back().get();
};

IGraphicalInstance* Scene::createModel(const Vector3m& position, const std::string& model) {
	// TODO à changer : il faut obtenir le IMeshLoader avec irrlicht

	// irr::scene::ISceneLoader::loadScene(model, _scenemanager);

	/*irr::scene::IAnimatedMesh* _animated_mesh = irr::scene::IMeshLoader::createMesh(model);
	irr::scene::IMesh* _mesh = _animated_mesh->getMesh(0);
	irr::scene::IMeshSceneNode * meshnode = _scenemanager->addMeshSceneNode(_mesh);
	meshnode->setID(_objectId);
	_listeObjet.push_back(Object(_objectId, meshnode));
	Scene::ChangePosition(SimulationToIrrlicht::VectorIrr(position), _objectId);
	incrementId();*/

	return createDefaultObject();
}

void Scene::remove(IGraphicalInstance* object) {
	// TODO On peut pas faire comme ça, parce que IGraphicalInstance ne possède  [...]
	// pas les méthodes de Object. Une manière propre de récupérer l'objet
	// est de récuperer son Id, puis de récuperer l'objet dans la liste qui
	// correspond à cet Id.

	/*object.getInternalPtr()->remove();
	int id = object->getId();
	_listeObjet.erase(_listeObjet.begin() + id);*/
}

void Scene::ChangePosition(irr::core::vector3df position, int id) {
	_listeObjet[id]->getInternalPtr()->setPosition(position);
};

void Scene::Rotation(irr::core::vector3df rotation, int id) {
	_listeObjet[id]->getInternalPtr()->setRotation(rotation);
};

// Pas besoin pour l'instant
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
	_scenemanager->addCameraSceneNodeMaya(0, rotatespeed, zoomspeed, translationspeed, -1, 40.f);
};

void Scene::PutCameraObjet() {
	_scenemanager->getActiveCamera()->setTarget(_listeObjet.at(0)->getInternalPtr()->getAbsolutePosition());
};

void Scene::incrementId() {

	_objectId++;
}

//
// Created by denis on 26/01/17.
//

#include "Scene.h"
#include "AxisSceneNode.h"
#include "IrrlichtUtils.h"
#include "SimulationToIrrlicht.h"

#include <log/Log.h>

// TODO Intégrer irrlicht dans la fenêtre gtk

Scene::Scene() : _objectId(0) {
	_device = irr::createDevice(irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(800, 600), 32, false, true, false, this);
	_scenemanager = _device->getSceneManager();
	_driver = _device->getVideoDriver();
	_collisions = _scenemanager->getSceneCollisionManager();

	// Camera
	AddCameraMaya(-1000.f, 200.f, 100.f);
	_scenemanager->getActiveCamera()->setTarget(irr::core::vector3df(15.f, 0.f, 10.f));
	AddAxis();

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

void Scene::AddAxis() {
	auto* axis = new AxesSceneNode(_scenemanager->getRootSceneNode(), _scenemanager, -1);
	axis->setAxesScale(10);
}

void Scene::update() {
	if(_device->run()) {
		// Mise à jour du ratio de la fenetre (si l'utilisateur la redimensionne)
		auto screenSize = _driver->getScreenSize();
		_scenemanager->getActiveCamera()->setAspectRatio(static_cast<float>(screenSize.Width) / screenSize.Height);

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
	auto* node = _scenemanager->addEmptySceneNode(0, _objectId);
	return &setupAndAddObject(node, {});
};

IGraphicalInstance* Scene::createCuboid(const Vector3m& position, const Vector3m& dimensions) {
	irr::scene::IMeshSceneNode* cube =
	    _scenemanager->addCubeSceneNode(1, 0, -1, SimulationToIrrlicht::VectorIrr(position), irr::core::vector3df(0, 180, 0));
	Object& obj = setupAndAddMeshObject(cube, position);

	irr::scene::IMesh* cubeMesh = cube->getMesh();
	irr::scene::IMeshManipulator* Mcube = _scenemanager->getMeshManipulator();
	Mcube->scale(cubeMesh, SimulationToIrrlicht::VectorIrr(dimensions));

	return &obj;
};

IGraphicalInstance* Scene::createCylinder(const Vector3m& position, Length radius, Length height) {
	const irr::scene::IGeometryCreator* cylindreGeo = _scenemanager->getGeometryCreator();
	irr::scene::IMesh* cylindreMesh = cylindreGeo->createCylinderMesh(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                  SimulationToIrrlicht::ToIrrlicht(height),
	                                                                  irr::u32(10),
	                                                                  irr::video::SColor(255, 0, 0, 255),
	                                                                  true,
	                                                                  0.0);

	auto* meshNodeTube = _scenemanager->addMeshSceneNode(cylindreMesh);
	return &setupAndAddMeshObject(meshNodeTube, position);
};

IGraphicalInstance* Scene::createSphere(const Vector3m& position, Length radius) {
	irr::scene::IMeshSceneNode* sphere = _scenemanager->addSphereSceneNode(SimulationToIrrlicht::ToIrrlicht(radius),
	                                                                       32,
	                                                                       0,
	                                                                       -1,
	                                                                       SimulationToIrrlicht::VectorIrr(position),
	                                                                       irr::core::vector3df(0, 0, 0),
	                                                                       irr::core::vector3df(1.0f, 1.0f, 1.0));

	return &setupAndAddMeshObject(sphere, position);
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

Object& Scene::setupAndAddMeshObject(irr::scene::IMeshSceneNode* node, const Vector3m& position) {
	Object& result = setupAndAddObject(node, position);

	// Add triangle selector
	irr::scene::ITriangleSelector* selector = _scenemanager->createTriangleSelector(node->getMesh(), node);
	node->setTriangleSelector(selector);
	selector->drop();

	return result;
}

Object& Scene::setupAndAddObject(irr::scene::ISceneNode* node, const Vector3m& position) {
	node->setID(_objectId);
	node->setPosition(SimulationToIrrlicht::VectorIrr(position));

	_listeObjet.push_back(std::make_unique<Object>(_objectId, node));
	incrementId();
	return *_listeObjet.back();
}

void Scene::remove(IGraphicalInstance* object) {
	for(auto it = _listeObjet.begin(); it != _listeObjet.end(); it++) {
		if((*it)->getId() == object->getId()) {
			(*it)->getInternalPtr()->remove();
			_listeObjet.erase(it);
			return;
		}
	}
}

bool Scene::OnEvent(const irr::SEvent& event) {
	if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
		if(event.MouseInput.Event == irr::EMIE_LMOUSE_PRESSED_DOWN) {
			// get camera direction
			// auto ray = _collisions->getRayFromScreenCoordinates({event.MouseInput.X, event.MouseInput.Y});
			core::line3df ray({1.5, 1000, 1}, {1.5, -1000, 1});
			core::vector3df point;
			core::triangle3df triangle;
			auto* s = _collisions->getSceneNodeAndCollisionPointFromRay(ray, point, triangle);

			/*std::cout << "Ray is " << ray.start << " ; " << ray.end << std::endl;
			std::cout << "Clicking on " << point.X << ", " << point.Y << ", " << point.Z << std::endl;
			std::cout << "Object is: " << s << std::endl;*/
		}
	}

	return false;
}

// Pas besoin pour l'instant
void Scene::Loop() {
	while(_device->run()) {
		_driver->beginScene(true, true, irr::video::SColor(255, 255, 255, 255));
		_scenemanager->drawAll();
		_driver->endScene();
	}
};

void Scene::AddCamera() {
	if(_camera != nullptr)
		_camera->remove();

	_camera = _scenemanager->addCameraSceneNode(0, irr::core::vector3df(-5, -5, -5), irr::core::vector3df(5, 0, 0));
};

void Scene::AddCameraMaya(irr::f32 rotatespeed, irr::f32 zoomspeed, irr::f32 translationspeed) {
	if(_camera != nullptr)
		_camera->remove();

	_camera = _scenemanager->addCameraSceneNodeMaya(0, rotatespeed, zoomspeed, translationspeed, -1, 40.f);
};

void Scene::cameraLookAt() {
	_scenemanager->getActiveCamera()->setTarget(_listeObjet.at(0)->getInternalPtr()->getAbsolutePosition());
};

void Scene::incrementId() {
	_objectId++;
}

Object& Scene::getAt(int id) {
	for(auto it = _listeObjet.begin(); it != _listeObjet.end(); it++) {
		if((*it)->getId() == id) {
			return **it;
		}
	}
	logError("Id not found: ", id);
	throw std::runtime_error("bad id");
}

#include "stdafx.h"
#include "Scene.h"
#include "CameraSystem.h"
#include "ResourcesManager.h"
#include "materials.h"
#include "PlayerInputSystem.h"

#include <iostream>     // cout, endl
#include <fstream>      // fstream

#include <algorithm>    // copy
#include <iterator>     // ostream_operator
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

typedef tokenizer< escaped_list_separator<char> > Tokenizer;
vector< string > vec;
string line;

Scene::Scene()
{
	children = new std::vector<Entity*>();

	ResourcesManager* resources = &ResourcesManager::getResourcesManager();



	//_entity2->set_scale(makeVector3(100.0f, 100.0f, 100.0f));

	//Entity* _entity = new Entity((resources->getVertexBufferArray()->at(1)), makeVector3((0.0f), 0.0f, (0.0f)));
	//_entity->army = 10;
	//_entity->id = 50;
	//_entity->set_rotationVelocity(makeVector3((0.0f), 0.00f, (0.0f)));
	//_entity->set_scale(makeVector3((1.0f), 1.0f, (1.0f)));
	//_entity->set_scaleVelocity(makeVector3((0.0f), 0.05f, (0.0f)));
	//children->push_back(_entity);

	Entity* _entity2 = new Entity((resources->getVertexBufferArray()->at(1)), makeVector3((0.0f), 0.0f, (0.0f)));
	_entity2->army = 10;
	_entity2->id = 50;
	_entity2->set_rotationVelocity(makeVector3((0.0f), 0.00f, (0.0f)));
	_entity2->set_scale(makeVector3((0.0f), 0.00f, (0.0f)));
	//_entity->set_scaleVelocity(makeVector3((0.0f), 0.05f, (0.0f)));
	children->push_back(_entity2);

	Entity* _entity3 = new Entity((resources->getVertexBufferArray()->at(0)), makeVector3(0.0f, 0.0f, 0.0f));
	_entity3->army = -1;
	_entity3->id = -1;
	_entity3->set_velocity(makeVector3(0.0f, 0.0f, 0.0f));
	_entity3->set_rotationVelocity(makeVector3((0.0f), 0.05f, (0.0f)));
	_entity3->set_scale(makeVector3((1.0f), 1.0f, (1.0f)));
	children->push_back(_entity3);

	////////////////

	Entity* camera = new Entity(NULL, makeVector3(
		00.0f, -50.0f, -50.0f));
	camera->army = 10;
	camera->id = 50;
	camera->set_eyeVector(normalizeVector3(makeVector3(1.0f, 1.0f, 1.0f)));
	camera->set_upVector(makeVector3(0.0f, 1.0f, 0.0f));
	//camera->set_velocity(makeVector3(0.01f, 0.00f, 0.01f));

	children->push_back(camera);

	PlayerInputSystem *playerInputSystem = &PlayerInputSystem::getPlayerInputSystem();
	playerInputSystem->setCurrentPlayer(camera);

	CameraSystem* cameraSystem = &CameraSystem::getCameraSystem();
	cameraSystem->setCurrentCamera(camera);



	_lights = new std::vector<light*>();
	/*light* li = new light((resources->getVertexBufferArray()->at(1)), makeVector3(0.0f, 0.0f, 10.0f), makeVector3(0.3f, 0.3f, 0.3f), makeVector3(1.0f, 1.0f, 1.0f), makeVector3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	li->set_velocity(makeVector3(-0.05f, -0.05f,0.0f));
	li->set_scale(makeVector3(2.0f, 2.0f, 2.0f));
	_lights->push_back(li);

	light* li2 = new light((resources->getVertexBufferArray()->at(1)), makeVector3(-100.0f, -100.0f, 10.0f), makeVector3(0.3f, 0.3f, 0.3f), makeVector3(1.0f, 1.0f, 1.0f), makeVector3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	li2->set_velocity(makeVector3(0.05f, 0.05f, 0.0f));
	li2->set_scale(makeVector3(2.0f, 2.0f, 2.0f));
	_lights->push_back(li2);

	light* li3 = new light((resources->getVertexBufferArray()->at(1)), makeVector3(-100.0f, -100.0f, 10.0f), makeVector3(0.3f, 0.3f, 0.3f), makeVector3(1.0f, 1.0f, 1.0f), makeVector3(1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f);
	li3->set_velocity(makeVector3(-0.05f, 0.05f, 0.0f));
	li3->set_scale(makeVector3(2.0f, 2.0f, 2.0f));
	_lights->push_back(li3);
	li->set_scale(makeVector3(0.0f, 0.0f, 0.0f));
	li2->set_scale(makeVector3(0.0f, 0.0f, 0.0f));
	li3->set_scale(makeVector3(0.0f, 0.0f, 0.0f));
	children->push_back(li);
	children->push_back(li2);
	children->push_back(li3);*/
}


char* Scene::loadTextFromFile(char*file)
{
	FILE* currentFile = fopen(file, "rt");
	fseek(currentFile, 0, SEEK_END);
	int count = (int)ftell(currentFile);

	rewind(currentFile);
	char* data = (char*)malloc(sizeof(char)*(count + 1));
	count = (int)fread(data, sizeof(char), count, currentFile);

	data[count] = '\0';
	fclose(currentFile);

	return data;

}

Scene::~Scene()
{
	for (std::vector<Entity*>::iterator iterator = children->begin(); iterator != children->end(); iterator++)
	{
		delete *iterator;
	}
}

std::vector<Entity *>* Scene::getChildren()
{
	return children;
}

std::vector<light *>* Scene::getLights()
{
	return _lights;
}

void Scene::createParticle(Vector3 position, float deltaTime)
{
	ResourcesManager* resources = &ResourcesManager::getResourcesManager();

	Entity* _entity = new Entity((resources->getVertexBufferArray()->at(3)), position);
	_entity->id = -1;
	_entity->isLight = true;
	_entity->set_scale(makeVector3(0.5f, 0.5f, 0.5f));
	_entity->lightLifetime = deltaTime;
	children->push_back(_entity);
}
#include "stdafx.h"
#include "CameraSystem.h"
#include "RenderSystem.h"
#include "PlayerInputSystem.h"

Entity* CameraSystem::getCurrentCamera()
{
	return _currentCamera;
}

void CameraSystem::setCurrentCamera(Entity* entity)
{
	_currentCamera = entity;

	RenderSystem* renderSystem = &RenderSystem::getRenderSystem();
	renderSystem->setCurrentCamera(_currentCamera);
	PlayerInputSystem* playerSystem = &PlayerInputSystem::getPlayerInputSystem();
	playerSystem->setCurrentPlayer(_currentCamera);
}

CameraSystem& CameraSystem::getCameraSystem()
{
	static CameraSystem* cameraSystem = NULL;

	if (cameraSystem == NULL) {
		cameraSystem = new CameraSystem();
	}

	return*cameraSystem;
}

void CameraSystem::destroyCameraSystem()
{
	CameraSystem* cameraSystem = &getCameraSystem();
	delete cameraSystem;
}

CameraSystem::CameraSystem()
{
}

CameraSystem::~CameraSystem()
{
}

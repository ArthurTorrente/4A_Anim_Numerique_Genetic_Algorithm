#pragma once


#ifndef _SimpleFps_CameraSystem
#define _SimpleFps_CameraSystem


#include "Entity.h"
#include "stdafx.h"

class CameraSystem
{
private:

	Entity* _currentCamera;

	CameraSystem();
	~CameraSystem();
public:
	Entity* getCurrentCamera();
	void setCurrentCamera(Entity* entity);

	static CameraSystem& getCameraSystem();
	static void destroyCameraSystem();
};

#endif
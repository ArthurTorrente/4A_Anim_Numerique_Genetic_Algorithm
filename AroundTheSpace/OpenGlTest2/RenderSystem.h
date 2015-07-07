#pragma once
#include <iostream>
#include <Windows.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "GLU32.lib")
#include<GL/glew.h>
#include<GLFW\glfw3.h>
#include<GL\GLU.h>

#ifndef _SimpleFps_RenderSystem_
#define _SimpleFps_RenderSystem_

#include "ShaderInterface.h"
#include "VertexBuffer.h"
#include <vector>
#include "Entity.h"
#include "light.h"
#include "CameraSystem.h"
#include "cinder/ImageIo.h"

using namespace ci;

class RenderSystem
{
private:
	RenderSystem();
	~RenderSystem();

	GLFWwindow* _window;

	CameraSystem* _cameraSystem;
	Entity* _currentCamera;
	

public:

	Entity* getCurrentCamera();
	void setCurrentCamera(Entity* currentCamera);

	GLuint _cubemaptexture;

	GLuint loadCubemap(vector<const GLchar*> faces);

	void render(std::vector<Entity*> *children, std::vector<light *>* lights);
	void renderDepth(std::vector<Entity*> *children, std::vector<light *>* lights);
	void renderLights(Entity* entity,std::vector<light *>* lights);
	void screenshot(char filename[160], int x, int y);
	void setMatrices(Entity* entity, ShaderInterface* shader);

	static RenderSystem& getRenderSystem();
	static void destroyRenderSystem();

	bool up_down = false;
	float next_Step = 180.0f;

	GLuint depthMapFBO;
	GLuint depthMap;
	void RenderQuad();

	cinder::Surface _m_algoGenImage;

	bool cinderImage = false;
	bool shadowDepth = false;
};

#endif


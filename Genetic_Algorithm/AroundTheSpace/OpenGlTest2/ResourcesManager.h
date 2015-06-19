#pragma once
#include <iostream>
#include <vector>
#include "ShaderInterface.h"
#include "VertexBuffer.h"

#ifndef _SimpleFps_ResourcesManager
#define _SimpleFps_ResourcesManager

class ResourcesManager
{
public:
	ResourcesManager();
	~ResourcesManager();

	std::vector<ShaderInterface*>* getShaderArray();
	std::vector<VertexBuffer*>* getVertexBufferArray();

	static ResourcesManager& getResourcesManager();
	static void destroyResourcesManager();

private:
	std::vector<ShaderInterface*> *_shaderArray;
	std::vector<VertexBuffer*> *_vertexBufferArray;

	

	GLuint textureObj;

	ShaderData* _shaderData;

	
};

#endif

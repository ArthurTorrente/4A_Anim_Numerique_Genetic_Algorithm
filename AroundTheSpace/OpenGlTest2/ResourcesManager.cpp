#include "stdafx.h"
#include "ResourcesManager.h"
#include "TriangleVertices.h"
#include "CubeVertices.h"
#include "verticesTiangle.h"
#include "CubeQuadTex.h"
#include "model.h"
#include "Planet.h"

ResourcesManager::ResourcesManager()
{
	_shaderArray = new std::vector < ShaderInterface* >;

	ShaderInterface *textureShader = new ShaderInterface("VertexShaderTexture.vs", "FragmentShaderTexture.fs");
	_shaderArray->push_back(textureShader);

	ShaderInterface *instancingShader = new ShaderInterface("instancing.vs", "instancing.frag");
	_shaderArray->push_back(instancingShader);

	ShaderInterface *lightShader = new ShaderInterface("light.vs", "light.frag");
	_shaderArray->push_back(lightShader);

	ShaderInterface *textShader = new ShaderInterface("TextRender.vs", "TextRender.frag");
	_shaderArray->push_back(textShader);

	ShaderInterface* skyboxShader = new ShaderInterface("skybox.vs", "skybox.frag");
	_shaderArray->push_back(skyboxShader);

	ShaderInterface* depthShader = new ShaderInterface("shadow_mapping_depth.vs", "shadow_mapping_depth.frag");
	_shaderArray->push_back(depthShader);

	ShaderInterface* depthDebugShader = new ShaderInterface("depthMap.vs", "depthMap.frag");
	_shaderArray->push_back(depthDebugShader);

	ShaderInterface* depthShaderinstancing = new ShaderInterface("shadow_mapping_depth_instancing.vs", "shadow_mapping_depth_instancing.frag");
	_shaderArray->push_back(depthShaderinstancing);

	ShaderInterface* depthDebugShaderinstancing = new ShaderInterface("depthMap_instancing.vs", "depthMap_instancing.frag");
	_shaderArray->push_back(depthDebugShaderinstancing);

	_shaderData = new ShaderData(makeVector4(1.0f, 0.0f, 1.0f, 1.0f), makeVector3(1.0f, 0.0f, 1.0f));


	_vertexBufferArray = new std::vector<VertexBuffer*>();

	Planet p;

	/*VertexBuffer* _planetVertexBuffer = new VertexBuffer(p.Vnu, p.Vnu.size() * sizeof(p.Vnu) * 2, GL_TRIANGLES, p.triangles.size(), sizeof(VertexDataPNT), _shaderArray->at(1), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), p.triangles, 300000);
	_vertexBufferArray->push_back(_planetVertexBuffer);*/

	VertexBuffer* _planetVertexBuffer = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(1), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)), p.triangles, 1000000, _shaderArray->at(8));
	_vertexBufferArray->push_back(_planetVertexBuffer);

	/*VertexBuffer* _vertexBufftext2 = new VertexBuffer(verticesCubeWithTexture, sizeof(verticesCubeWithTexture), GL_TRIANGLES, 36, sizeof(VertexDataPNT), _shaderArray->at(6), _shaderData, (GLvoid*)(offsetof(VertexDataPNT, positionCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, normalCoordinates)), (GLvoid*)(offsetof(VertexDataPNT, textureCoordinates)));
	_vertexBufferArray->push_back(_vertexBufftext2);*/

	VertexBuffer* _vertexBufftext3 = new VertexBuffer(skyboxVertices, sizeof(skyboxVertices), GL_TRIANGLES, 36, sizeof(VertexDataP), _shaderArray->at(4), _shaderData, (GLvoid*)(offsetof(VertexDataP, positionCoordinates)));
	_vertexBufferArray->push_back(_vertexBufftext3);
	/*_vertexBufftext3->_shaderDepth = depthShader;*/
	_planetVertexBuffer->_shaderDepth = depthShaderinstancing;
	//_vertexBufftext2->_shaderDepth = depthShader;
	_planetVertexBuffer->_shaderDepthFramed = _shaderArray->at(8);
	//_vertexBufftext2->_shaderDepthFramed = _shaderArray->at(8);
	std::vector<materials*> *_materials = new std::vector<materials*>();
	_materials->push_back(new materials("manu5.png", 32.0f));

	//_vertexBufftext3->set_materials(_materials);

	_planetVertexBuffer->set_materials(_materials);
	//_vertexBufftext2->set_materials(_materials);
}


ResourcesManager::~ResourcesManager()
{
	for (std::vector<ShaderInterface* >::iterator it = _shaderArray->begin(); it != _shaderArray->end(); it++)
	{
		delete *it;
	}

	delete _shaderArray;

	for (std::vector<VertexBuffer* >::iterator it = _vertexBufferArray->begin(); it != _vertexBufferArray->end(); it++)
	{
		delete *it;
	}

	delete _shaderData;
	delete _vertexBufferArray;
}



std::vector<ShaderInterface*>* ResourcesManager::getShaderArray()
{
	return _shaderArray;
}

std::vector<VertexBuffer*>* ResourcesManager::getVertexBufferArray()
{
	return _vertexBufferArray;
}

ResourcesManager& ResourcesManager::getResourcesManager()
{
	static ResourcesManager * resourcesManager = NULL;

	if (resourcesManager == NULL)
	{
		resourcesManager = new ResourcesManager();
	}

	return *resourcesManager;
}

void ResourcesManager::destroyResourcesManager()
{
	ResourcesManager * resourcesManager = &getResourcesManager();
	delete resourcesManager;
}

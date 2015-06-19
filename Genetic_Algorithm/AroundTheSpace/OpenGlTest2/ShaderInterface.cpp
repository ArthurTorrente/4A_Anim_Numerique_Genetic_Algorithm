#include "stdafx.h"
#include "ShaderInterface.h"
#include <stdio.h>
#include <stdlib.h>

GLint ShaderInterface::getProgramHandle()
{
	return shader->getProgramHandle();
}
GLint ShaderInterface::get_aPositionVertex()
{
	return _aPositionVertex;
}

GLint ShaderInterface::get_aPositionNormals()
{
	return _aPositionNormals;
}

GLint ShaderInterface::get_uColor()
{
	return _uColor;
}

GLint ShaderInterface::get_uLightPosition()
{
	return _uLightPosition;
}

GLint ShaderInterface::get_aTextCoords()
{
	return _aTextCoords;
}

GLint ShaderInterface::get_uinstanceMatrix()
{
	return _uinstanceMatrix;
}

ShaderInterface::ShaderInterface(char*VS, char*FS)
{
	_VertexShaderString = loadTextFromFile(VS);
	_fragmentShaderString = loadTextFromFile(FS);

	shader = new ShaderLoader(_VertexShaderString, _fragmentShaderString);

	free(_VertexShaderString);
	free(_fragmentShaderString);

	_uinstanceMatrix = glGetAttribLocation(shader->getProgramHandle(), "instanceMatrix");

	_aPositionVertex = glGetAttribLocation(shader->getProgramHandle(), "aPositionVertex");
	_aPositionNormals = glGetAttribLocation(shader->getProgramHandle(), "aPositionNormal");

	_aTextCoords = glGetAttribLocation(shader->getProgramHandle(), "aTextCoords");

	_uColor = glGetUniformLocation(shader->getProgramHandle(), "uColor");
	_uLightPosition = glGetUniformLocation(shader->getProgramHandle(), "uLightPosition");

	
}
ShaderInterface::~ShaderInterface()
{
	delete shader;
}

char* ShaderInterface::loadTextFromFile(char*file)
{
	FILE* currentFile = fopen(file, "rt");
	fseek(currentFile,0, SEEK_END);
	int count = (int)ftell(currentFile);

	rewind(currentFile);
	char* data = (char*)malloc(sizeof(char)*(count + 1));
	count = (int)fread(data, sizeof(char), count, currentFile);

	data[count] = '\0';
	fclose(currentFile);

	return data;

}
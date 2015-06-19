#pragma once
#include "ShaderLoader.h"
class ShaderInterface
{
private :
	ShaderLoader* shader;

	GLint _aPositionVertex;
	GLint _aPositionNormals;
	GLint _uColor;
	GLint _uLightPosition;
	GLint _aTextCoords;
	GLint _uinstanceMatrix;

	char* _VertexShaderString;
	char* _fragmentShaderString;

	char* loadTextFromFile(char* file);

public :

	GLint getProgramHandle();
	GLint get_aPositionVertex();
	GLint get_aPositionNormals();
	GLint get_uColor();
	GLint get_uLightPosition();
	GLint get_aTextCoords();
	GLint get_uinstanceMatrix();

	ShaderInterface(char*VS, char*FS);
	~ShaderInterface();
};
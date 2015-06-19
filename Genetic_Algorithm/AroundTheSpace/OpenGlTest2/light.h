#pragma once
#include <iostream>
#include"VertexBuffer.h"
#include"Vector3.h"
#include "Entity.h"
#include <vector>

/*glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].ambient"), 0.05f, 0.05f, 0.05f);
glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].diffuse"), 0.8f, 0.8f, 0.8f);
glUniform3f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].constant"), 1.0f);
glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].linear"), 0.09);
glUniform1f(glGetUniformLocation(_shader->getProgramHandle(), "pointLights[0].quadratic"), 0.032);*/

#ifndef _SimpleFps_Lights
#define _SimpleFps_Lights

class light : public Entity
{
private:
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	GLfloat constant;
	GLfloat linear;
	GLfloat quadratic;

public:

	light(VertexBuffer* newVertexBuffer, Vector3 newVector3, Vector3 _ambient, Vector3 _difuse, Vector3 _specular, GLfloat _constant, GLfloat _linear, GLfloat _quadratic);
	~light();

	void renderLight();

	Vector3 get_ambient();
	Vector3 get_diffuse();
	Vector3 get_specular();
	GLfloat get_constant();
	GLfloat get_linear();
	GLfloat get_quadratic();


};

#endif
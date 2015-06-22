#include "stdafx.h"
#include "light.h"


light::light(VertexBuffer* newVertexBuffer, Vector3 newVector3, Vector3 _ambient, Vector3 _difuse, Vector3 _specular, GLfloat _constant, GLfloat _linear, GLfloat _quadratic) : Entity(newVertexBuffer, newVector3), ambient(_ambient), diffuse(_difuse), specular(_specular), constant(_constant), linear(_linear), quadratic(_quadratic)
{
}

light::~light()
{
}

Vector3 light::get_ambient()
{
	return ambient;
}

Vector3 light::get_diffuse()
{
	return diffuse;
}

Vector3 light::get_specular()
{
	return specular;
}

GLfloat light::get_constant()
{
	return constant;
}

GLfloat light::get_linear()
{
	return linear;
}

GLfloat light::get_quadratic()
{
	return quadratic;
}
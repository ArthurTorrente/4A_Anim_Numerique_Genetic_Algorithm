#include "stdafx.h"
#include "ShaderData.h"

Vector4 ShaderData::get_uColorValue()
{
	return _uColorValue;
}

Vector3 ShaderData::get_uLightPosition()
{
	return _uLightPosition;
}

void ShaderData::set_uColorValue(Vector4 newColor)
{
	_uColorValue = newColor;
}

void ShaderData::set_uLightPosition(Vector3 newLightPosition)
{
	_uLightPosition = newLightPosition;
}

ShaderData::ShaderData(Vector4 newColor, Vector3 newlightPosition) : _uColorValue(newColor), _uLightPosition(newlightPosition)
{
}


ShaderData::~ShaderData()
{
}


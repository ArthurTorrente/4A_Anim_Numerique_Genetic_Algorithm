#pragma once
#include "Vector3.h"
#include "Vector4.h"

class ShaderData
{
private:

	Vector4 _uColorValue;
	Vector3 _uLightPosition;

public:

	Vector4 get_uColorValue();
	Vector3 get_uLightPosition();

	void set_uColorValue(Vector4 newColor);
	void set_uLightPosition(Vector3 newLightPosition);

	ShaderData(Vector4 newColor, Vector3 newlightPosition);
	~ShaderData();
};


#pragma once
#include "stdafx.h"
#include <iostream>
#include"VertexBuffer.h"
#include"Vector3.h"
#include <vector>
#include "materials.h"

#ifndef _SimpleFps_Entity
#define _SimpleFps_Entity

class Entity
{
private:

	VertexBuffer* _vertexBuffer;

	Vector3 _position;
	Vector3 _scale;
	Vector3 _rotation;

	Vector3 _velocity;
	Vector3 _scaleVelocity;
	Vector3 _rotationVelocity;

	Vector3 _eyeVector;
	Vector3 _upVector;

	Vector3 _goal;

public:

	VertexBuffer* get_vertexBuffer();
	void set_vertexBuffer(VertexBuffer* newVertexBuffer);

	Vector3 get_position();
	void set_position(Vector3 newVector3);

	Vector3 get_scale();
	void set_scale(Vector3 newScale);

	Vector3 get_rotation();
	void set_rotation(Vector3 newRotation);

	Vector3 get_velocity();
	void set_velocity(Vector3 newVelocity);

	Vector3 get_scaleVelocity();
	void set_scaleVelocity(Vector3 newRotationVelocity);

	Vector3 get_rotationVelocity();
	void set_rotationVelocity(Vector3 newrotationVelocity);

	Vector3 get_eyeVector();
	void set_eyeVector(Vector3 newEyeVector);

	Vector3 get_upVector();
	void set_upVector(Vector3 newUpVector);

	Vector3 get_goal();
	void set_goal(Vector3 newGoal);

	Entity(VertexBuffer* newVertexBuffer, Vector3 newVector3);
	~Entity();

	int army;
	int id;
	int chess;


	bool isLight;
	float lightLifetime;
};

#endif
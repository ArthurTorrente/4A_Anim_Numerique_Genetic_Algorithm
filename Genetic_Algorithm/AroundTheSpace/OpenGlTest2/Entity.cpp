#include "stdafx.h"
#include "Entity.h"

VertexBuffer* Entity::get_vertexBuffer()
{
	return _vertexBuffer;
}

void Entity::set_vertexBuffer(VertexBuffer* newVertexBuffer)
{
	_vertexBuffer = newVertexBuffer;
}

Vector3 Entity::get_position()
{
	return _position;
}

void Entity::set_position(Vector3 newVector3)
{
	_position = newVector3;
}

Vector3 Entity::get_scale()
{
	return _scale;
}

void Entity::set_scale(Vector3 newScale)
{
	_scale = newScale;
}

Vector3 Entity::get_rotation()
{
	return _rotation;
}

void Entity::set_rotation(Vector3 newRotation)
{
	_rotation = newRotation;
}

Vector3 Entity::get_velocity()
{
	return _velocity;
}

void Entity::set_velocity(Vector3 newVelocity)
{
	_velocity = newVelocity;
}

Vector3 Entity::get_scaleVelocity()
{
	return _scaleVelocity;
}

void Entity::set_scaleVelocity(Vector3 newScaleVelocity)
{
	_scaleVelocity = newScaleVelocity;
}

Vector3 Entity::get_rotationVelocity()
{
	return _rotationVelocity;
}

void Entity::set_rotationVelocity(Vector3 newRotationVelocity)
{
	_rotationVelocity = newRotationVelocity;
}

Vector3 Entity::get_eyeVector()
{
	return _eyeVector;
}

void Entity::set_eyeVector(Vector3 newEyeVector)
{
	_eyeVector = newEyeVector;
}

Vector3 Entity::get_upVector()
{
	return _upVector;
}

void Entity::set_upVector(Vector3 newUpVector)
{
	_upVector = newUpVector;
}

Vector3 Entity::get_goal()
{
	return _goal;
}

void Entity::set_goal(Vector3 newGoal)
{
	_goal = newGoal;
}

Entity::Entity(VertexBuffer* newVertexBuffer, Vector3 newVector3) : _vertexBuffer(newVertexBuffer), 
_position(newVector3), _scale(makeVector3(10.0f, 10.0f, 10.0f)), _rotation(makeVector3(0.0f, 0.0f, 0.0f)), 
_velocity(makeVector3(0.0f, 0.0f, 0.0f)), _scaleVelocity(makeVector3(0.0f, 0.0f, 0.0f)), _rotationVelocity(makeVector3(0.0f, 0.0f, 0.0f)),
_eyeVector(makeVector3(0.0f, 0.0f, 0.0f)), _upVector(makeVector3(0.0f, 1.0f, 0.0f)), _goal(newVector3)
{
	isLight = false;
}


Entity::~Entity()
{
}

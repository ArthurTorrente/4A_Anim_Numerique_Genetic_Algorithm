#include "stdafx.h"
#include "PlayerInputSystem.h"
#include "Matrix3.h"

void PlayerInputSystem::setCurrentPlayer(Entity *newPlayer)
{
	_currentPlayer = newPlayer;
	_eyeVector = normalizeVector3(newPlayer->get_eyeVector());
}

PlayerInputSystem::PlayerInputSystem() : _window(glfwGetCurrentContext())
{
	double x = _lastMousePosition.x;
	double y = _lastMousePosition.y;
	glfwGetCursorPos(_window, &x, &y);

}

PlayerInputSystem::~PlayerInputSystem()
{

}

void PlayerInputSystem::keyCallback(GLFWwindow *window,
	int key,
	int scancode,
	int action,
	int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		if (GLFW_CURSOR_DISABLED == glfwGetInputMode(glfwGetCurrentContext(), GLFW_CURSOR)) {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else {
			glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}

	}

}

void PlayerInputSystem::update()
{
	/*if (_currentPlayer == NULL){
		printf("no current player");
	}
	if (_currentPlayer != NULL &&
		glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {

		if (glfwGetKey(_window, GLFW_KEY_Z)) {
			
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(),makeVector3(0.00f,0.00f,-0.07f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.0f, 0.00f, 0.07f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_S)) {

			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(0.00f, 0.00f, 0.07f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.0f, 0.00f, -0.07f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_Q)) {

			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(-0.07f, 0.00f, 0.0f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(-0.07f, 0.00f, 0.00f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_D)) {
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(0.07f, 0.00f, 0.0f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.07f, 0.00f, 0.00f)));
		}
		if (glfwGetKey(_window, GLFW_KEY_SPACE)) {

			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(0.00f, 0.07f, 0.0f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.00f, 0.07f, 0.00f)));
		}
		if (glfwGetKey(_window, GLFW_KEY_LEFT_CONTROL)) {

			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), makeVector3(0.00f, -0.07f, 0.0f)));
			_currentPlayer->set_goal(_currentPlayer->get_position());
			_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_eyeVector(), makeVector3(0.00f, -0.07f, 0.00f)));
		}*/
		/*Vector2 currentMousePosition;
		double x;
		double y;
		glfwGetCursorPos(_window, &x, &y);
		GLfloat x3 = x;
		GLfloat y3 = y;
		currentMousePosition.x = x3;
		currentMousePosition.y = y3;

		_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*(-(currentMousePosition.x - _lastMousePosition.x)), 0.0f, 1.0f, 0.0f));

		double x2 = _lastMousePosition.x;
		double y2 = _lastMousePosition.y;
		glfwGetCursorPos(_window, &x2, &y2);

		_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_position(), _eyeVector));*/
	if (_currentPlayer != NULL &&
		glfwGetInputMode(_window, GLFW_CURSOR) == GLFW_CURSOR_DISABLED) {

		if (glfwGetKey(_window, GLFW_KEY_W)) {
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), scalerMultiplyVector3(_eyeVector, 0.07f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_S)) {
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), scalerMultiplyVector3(_eyeVector, 0.07f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_A)) {
			_currentPlayer->set_position(subtractVector3(_currentPlayer->get_position(), scalerMultiplyVector3(crossProductVector3(_eyeVector, makeVector3(0.0f, 1.0f, 0.0f)), 0.07f)));
		}

		if (glfwGetKey(_window, GLFW_KEY_D)) {
			_currentPlayer->set_position(addVector3(_currentPlayer->get_position(), scalerMultiplyVector3(crossProductVector3(_eyeVector, makeVector3(0.0f, 1.0f, 0.0f)), 0.07f)));
		}

		Vector2 currentMousePosition;
		double x;
		double y;
		glfwGetCursorPos(_window, &x, &y);
		GLfloat x3 = x;
		GLfloat y3 = y;
		currentMousePosition.x = x3;
		currentMousePosition.y = y3;

		_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*(-(currentMousePosition.x - _lastMousePosition.x)), 0.0f, 1.0f, 0.0f));
		_eyeVector = transformVector3(_eyeVector, makeRotationMatrix3((3.14f / (4.0f*45.0f))*(-(currentMousePosition.y - _lastMousePosition.y)), 1.0f,0.0f, 0.0f));

		double x2; //= _lastMousePosition.x;
		double y2; //= _lastMousePosition.y;
		glfwGetCursorPos(_window, &x2, &y2);
		_lastMousePosition.x = x2;
		_lastMousePosition.y = y2;
		_currentPlayer->set_eyeVector(addVector3(_currentPlayer->get_position(), _eyeVector));



	}
}

void PlayerInputSystem::keyCallbackFun(GLFWwindow *window,
	int key,
	int scancode,
	int action,
	int mods)
{
	PlayerInputSystem *playerInputSystem = &getPlayerInputSystem();
	playerInputSystem->keyCallback(window, key, scancode, action, mods);
}

PlayerInputSystem& PlayerInputSystem::getPlayerInputSystem()
{
	static PlayerInputSystem *playerInputSystem = NULL;

	if (playerInputSystem == NULL) {

		glfwSetKeyCallback(glfwGetCurrentContext(), *keyCallbackFun);
		glfwSetInputMode(glfwGetCurrentContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		playerInputSystem = new PlayerInputSystem();
	}

	return *playerInputSystem;
}

void PlayerInputSystem::destroyPlayerInputSystem()
{
	PlayerInputSystem *playerInputSystem = &getPlayerInputSystem();
	delete playerInputSystem;
}
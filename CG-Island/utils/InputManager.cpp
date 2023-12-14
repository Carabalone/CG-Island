#include "InputManager.h"

#include <iostream>

InputManager::InputManager()
{
	keys.fill(InputState::Released);
	mouseButtons.fill(InputState::Released);
	deltaX = 0.0f;
	deltaY = 0.0f;
	lastX = 0.0f;
	lastY = 0.0f;

	scrollPos = glm::vec2(0.0f);
	scrollDelta = glm::vec2(0.0f);
}

InputManager::~InputManager() {
	// EMPTY
}

InputManager::InputState InputManager::getKeyState(int key) {
	return keys[key];
}

bool InputManager::validKey(int key) const {
	return key >= 0 && key < GLFW_KEY_LAST;
}

bool InputManager::validMouseButton(int mb) const {
	return mb >= 0 && mb < GLFW_KEY_LAST;
}

void InputManager::updateKeyState(int key, int action)
{
	if (validKey(key))
	{

		if (action == GLFW_REPEAT) {
			keys[key] = InputState::Held;
		}
		else if (action == GLFW_PRESS)
		{
			keys[key] = InputState::Pressed;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = InputState::Released;
		}
	}
}

void InputManager::updateMouseButtonState(int button, int action)
{
	if (validMouseButton(button))
	{

		if (action == GLFW_REPEAT) {
			mouseButtons[button] = InputState::Held;
		}
		else if (action == GLFW_PRESS)
		{
			mouseButtons[button] = InputState::Pressed;
		}
		else if (action == GLFW_RELEASE)
		{
			mouseButtons[button] = InputState::Released;
		}
	}
}

bool InputManager::isKeyPressed(int key) const
{
	return validKey(key) && keys[key] == InputState::Pressed;
}

bool InputManager::isKeyReleased(int key) const
{
	return validKey(key) && keys[key] == InputState::Released;
}

bool InputManager::isKeyHeld(int key) const
{
	return validKey(key) && keys[key] == InputState::Held;
}

std::string InputManager::stateToString(InputState state) {
	if (state == InputManager::Held) return "Held";
	if (state == InputManager::Pressed) return "Pressed";
	if (state == InputManager::Released) return "Released";
}

void InputManager::updateMousePosition(double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	// Add your logic for handling mouse movement here
	deltaX = xpos - lastX;
	deltaY = lastY - ypos; // Reversed since y-coordinates range from bottom to top

	// Add any additional logic you need with deltaX and deltaY

	lastX = xpos;
	lastY = ypos;
}

InputManager::InputState InputManager::getMouseButtonState(int button)
{
	if (validMouseButton(button))
	{
		return mouseButtons[button];
	}
	return InputState::Released;
}

bool InputManager::isMouseButtonPressed(int button) const
{
	return validMouseButton(button) && mouseButtons[button] == InputState::Pressed;
}

bool InputManager::isMouseButtonReleased(int button) const
{
	return validMouseButton(button) && mouseButtons[button] == InputState::Released;
}

bool InputManager::isMouseButtonHeld(int button) const
{
	return validMouseButton(button) && mouseButtons[button] == InputState::Held;
}

bool InputManager::isMouseButtonHeldOrPressed(int button) const
{
	return isMouseButtonHeld(button) || isMouseButtonPressed(button);
}

void InputManager::getDeltaMousePos(float* outParam) {
	outParam[0] = deltaX;
	outParam[1] = deltaY;
}

void InputManager::updateScrollState(double xoffset, double yoffset)
{
	float deltaX = static_cast<float>(xoffset) - scrollPos.x;
	float deltaY = static_cast<float>(yoffset) - scrollPos.y;

	scrollPos.x = static_cast<float>(xoffset);
	scrollPos.y = static_cast<float>(yoffset);
	scrollDelta.x = deltaX;
	scrollDelta.y = deltaY;
}

void InputManager::getScrollDelta(float* outParam)
{
	outParam[0] = scrollDelta.x;
	outParam[1] = scrollDelta.y;
}

#pragma once
#include <array>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/common.hpp>
#include <glm/vec2.hpp>

#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H
class InputManager
{
public:
    enum InputState
    {
        Pressed,
        Released,
        Held,
    };

private:
    std::array<InputState, GLFW_KEY_LAST> keys;
    std::array<InputState, GLFW_MOUSE_BUTTON_LAST> mouseButtons;
    bool firstMouse = true;
    float deltaX, deltaY, lastX, lastY;
    glm::vec2 scrollPos, scrollDelta;

public:
    InputManager();
    ~InputManager();
    bool validMouseButton(int mb) const;
    void updateKeyState(int key, int action);
    void updateMouseButtonState(int button, int action);
    InputState getKeyState(int key);
    bool validKey(int key) const;
    bool isKeyPressed(int key) const;
    bool isKeyReleased(int key) const;
    bool isKeyHeld(int key) const;
    InputState getMouseButtonState(int button);
    bool isMouseButtonPressed(int button) const;
    bool isMouseButtonReleased(int button) const;
    bool isMouseButtonHeld(int button) const;
    bool isMouseButtonHeldOrPressed(int button) const;
    void getDeltaMousePos(float* outParam);
    std::string stateToString(InputState state);
    void updateMousePosition(double xpos, double ypos);
    void updateScrollState(double xoffset, double yoffset);
    void getScrollDelta(float* outParam);
};

#endif // INPUT_MANAGER_H

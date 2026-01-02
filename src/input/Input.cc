#include "input/Input.h"

GLFWwindow* Input::s_window = nullptr; // null at first

void Input::init(GLFWwindow* window) {
    s_window = window;
}

bool Input::isKeyPressed(int key) {
    auto state = glfwGetKey(s_window, key);

    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isMouseButtonPressed(int button) {
    auto state = glfwGetMouseButton(s_window, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::getMousePosition() {
    double xPos, yPos;
    glfwGetCursorPos(s_window, &xPos, &yPos);
    return glm::vec2(xPos, yPos);
}
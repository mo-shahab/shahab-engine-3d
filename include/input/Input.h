#ifndef INPUT_H
#define INPUT_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Input {
public:
    static void init(GLFWwindow* window);

    static bool isKeyPressed(int key);
    static bool isMouseButtonPressed(int button);
    static glm::vec2 getMousePosition();

private:
    static GLFWwindow* s_window;
};

#endif // INPUT_H
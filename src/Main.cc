#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "model.h"
#include "Shaders.h"
#include "Camera.h"

Camera myCamera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = 320, lastY = 240; // Center of screen
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        myCamera.processMouseMovement(xoffset, yoffset);
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    myCamera.processMouseScroll(yoffset);
}


int main() {

    GLFWwindow* window;

    if(!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if(!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGL(glfwGetProcAddress)) return -1;

    Model myModel("bugatti/bugatti.obj");
    Shader myShader("shaders/default.vert", "shaders/default.frag");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {

        // mouse movement
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // handling keyboard input
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) myCamera.processKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) myCamera.processKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) myCamera.processKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) myCamera.processKeyboard(RIGHT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) myCamera.processKeyboard(UP, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) myCamera.processKeyboard(DOWN, deltaTime);

        /* Render here */
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f); 
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        myShader.use();

        // these transormations are AI slop

        // 1. Projection: 45-degree field of view
        glm::mat4 projection = glm::perspective(glm::radians(myCamera.m_zoom), 640.0f / 480.0f, 0.1f, 100.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::scale(model, glm::vec3(0.5f)); // Try 0.5 instead of 0.05

        // camera/view transformation
        glm::mat4 view = myCamera.getViewMatrix();

        // model = glm::rotate(model, (float)glfwGetTime() * 0.5f, glm::vec3(0.0f, 1.0f, 0.0f));

        // Send these to your shader uniforms
        glUniformMatrix4fv(glGetUniformLocation(myShader.m_ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(myShader.m_ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(myShader.m_ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        myModel.drawModel();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();


    return 0;
}
#ifndef APPLICATION_H
#define APPLICATION_H

#include <glad/gl.h>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>

#include "core/Window.h"
#include "renderer/Shaders.h"
#include "renderer/Renderer.h"
#include "scene/Model.h"
#include "scene/Camera.h"
#include "input/Input.h"

class Application {
public: 
    Application(const std::string& title = "Shahab's Engine");
    virtual ~Application(); // let us force the derived class to implement its own destructor

    void run();
    void stop();

    Window& getWindow();

private: 
    std::unique_ptr<Window> m_mainWindow;
    bool m_isRunning;

    std::vector<std::unique_ptr<Model>> m_models; 
    std::unique_ptr<Shader> m_defaultShader;
    std::unique_ptr<Camera> m_camera;

    glm::mat4 m_viewMatrix;
    glm::mat4 m_projectionMatrix;

    void update(float deltaTime);
    void render();
};

#endif // APPLICATION_H
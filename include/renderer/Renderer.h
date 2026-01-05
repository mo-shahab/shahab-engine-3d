#ifndef RENDERER_H
#define RENDERER_H

#include <memory>
#include <glm/glm.hpp>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "renderer/Shaders.h"
#include "scene/Model.h"
#include "scene/Skybox.h"

class Renderer {
public:

    static void init();
    static void shutdown();

    // core drawing method
    static void submit(Shader& shader, Model& model, const glm::mat4& view, const glm::mat4& projection);


    // managing viewport and the frame on the screen
    static void clear(float r, float g, float b, float a =1.0f);
    static void setViewport(int x, int y, int width, int height);
    static void drawViewportGizmo(const glm::mat4& cameraRotation, const glm::mat4& projection);
    static void beginScene(glm::mat4& view, glm::mat4& projection);

private:
    static GLuint s_LineVAO, s_LineVBO;

    static std::unique_ptr<Shader> s_lineShader;
    static std::unique_ptr<Skybox> s_skybox;

    static void drawGrid(const glm::mat4& view, const glm::mat4& projection);
    static void drawAxes(const glm::mat4& view, const glm::mat4& projection);
    static void renderLine(glm::vec3 start, glm::vec3 end, glm::vec3 color, const glm::mat4& view, const glm::mat4& projection);


};

#endif // RENDERER_H
#ifndef RENDERER_H
#define RENDERER_H

#include <glm/glm.hpp>
#include <glad/gl.h>

#include "renderer/Shaders.h"
#include "scene/Model.h"

class Renderer {
public:
    static void init();
    static void shutdown();

    // core drawing method
    static void submit(Shader& shader, Model& model, const glm::mat4& view, const glm::mat4& projection);


    // managing viewport and the frame on the screen
    static void clear(float r, float g, float b, float a =1.0f);
    static void setViewport(int x, int y, int width, int height);

};

#endif // RENDERER_H
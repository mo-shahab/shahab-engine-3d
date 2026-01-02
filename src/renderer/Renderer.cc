#include "renderer/Renderer.h"

void Renderer::init() {
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::clear(float r, float g, float b, float a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::setViewport(int x, int y, int width, int height) {
    glViewport(x, y, width, height);
}

void Renderer::submit(Shader& shader, Model& model, const glm::mat4& view, const glm::mat4& projection) {
    shader.use();

    // set the view and projection matrices
    // in the shader
    shader.setMat4("u_View", view);
    shader.setMat4("u_Projection", projection);

    model.draw(shader);
}

void Renderer::shutdown() {
}
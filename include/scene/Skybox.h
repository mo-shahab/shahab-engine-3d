#ifndef SKYBOX_H
#define SKYBOX_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/gl.h>
#include <vector>
#include <string>
#include <memory>

#include "renderer/Shaders.h"
#include "scene/TextureLoader.h"

class Skybox {
public:
    Skybox(const std::vector<std::string>& faces);
    void draw(const glm::mat4& view, const glm::mat4& projection);

private:
    unsigned int m_VAO, m_VBO;
    unsigned int m_CubemapID;
    std::unique_ptr<Shader> m_shader;
};

#endif // SKYBOX_H
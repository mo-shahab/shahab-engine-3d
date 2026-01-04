#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>
#include <glad/gl.h>
#include <string>

#include "renderer/Shaders.h"
#include "scene/TextureLoader.h"

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public: 
        bool m_isVisible = true;
        std::string m_name;
        glm::vec4 m_baseColor = glm::vec4(1.0f); // Default to white

        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures, glm::vec4 baseColor, const std::string& name = "Unnamed Mesh");
        void drawMesh(Shader& shader);
        ~Mesh();

    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        std::vector<Texture> m_textures;

        unsigned int m_VAO, m_VBO, m_EBO;

        void setupMesh();
};

#endif // MESH_H
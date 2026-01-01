#ifndef MESH_H
#define MESH_H

#include <glm/glm.hpp>

#include <vector>
#include <glad/gl.h>

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;
};

class Mesh {
    public: 
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
        void drawMesh();
        ~Mesh();
    private:
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;
        unsigned int m_VAO, m_VBO, m_EBO;

        void setupMesh();
};

#endif // MESH_H
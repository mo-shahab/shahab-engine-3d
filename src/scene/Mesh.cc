#include "scene/Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices
    , std::vector<unsigned int> indices
    , std::vector<Texture> textures
    , glm::vec4 baseColor
    , const std::string& name
) 
    : m_vertices(vertices)
    , m_indices(indices)
    , m_textures(textures)
    , m_baseColor(baseColor)
    , m_name(name)
    , m_VAO(0)
    , m_VBO(0)
    , m_EBO(0) 
{
    setupMesh(); 
}

/**
 * @brief Sets up the mesh for rendering by initializing OpenGL buffers and configuring vertex attributes.
 *
 * This function generates and binds the necessary OpenGL objects for the mesh, including the Vertex Array Object (VAO),
 * Vertex Buffer Object (VBO), and Element Buffer Object (EBO). It uploads the mesh's vertex and index data to the GPU,
 * and specifies how the vertex data is laid out in memory for use in shaders.
 *
 * The function enables and configures three vertex attributes:
 *   - Position (location 0): 3 floats per vertex, starting at offset 0.
 *   - Normal (location 1): 3 floats per vertex, starting at the offset of the 'normal' member in the Vertex struct.
 *   - Texture Coordinates (location 2): 2 floats per vertex, starting at the offset of the 'texCoords' member in the Vertex struct.
 *
 * After configuration, the VAO is unbound to prevent accidental modification.
 */
void Mesh::setupMesh() {
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), &m_vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}

void Mesh::drawMesh(Shader& shader) {
    shader.use();
    shader.setBool("u_HasTexture", !m_textures.empty());

    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;

    for(unsigned int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i); 
        
        std::string number;
        std::string name = m_textures[i].type;
        
        if(name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if(name == "texture_specular")
            number = std::to_string(specularNr++);

        // This sets "texture_diffuse1" to 0, "texture_diffuse2" to 1, etc.
        shader.setInt((name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }

    glBindVertexArray(m_VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0); // Clean up
}

Mesh::~Mesh() {
    // glDeleteVertexArrays(1, &m_VAO);
    // glDeleteBuffers(1, &m_VBO);
    // glDeleteBuffers(1, &m_EBO);
}
#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <cctype>
#include <vector>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

#include "renderer/Shaders.h"
#include "scene/Mesh.h"

class Model {
public:
    // filePath is the path to the 3D model file
    const aiScene* m_scene;
    unsigned int m_numMeshes;

    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    aiNode* m_rootNode;
    std::vector<Mesh> m_meshes;

    Model(const std::string& filePath);

    void draw(Shader& shader);
    std::string getName() const;

    ~Model();

private: 
    std::string m_filePath;
    Assimp::Importer m_importer;

    void loadModel(const std::string& path);
    void processMeshes();
    void drawModel();
};

#endif // MODEL_H
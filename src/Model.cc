#include "model.h"

// implementing the constructor
Model::Model(const std::string& filePath)
: 
    m_filePath(filePath),
    m_scene(nullptr),
    m_rootNode(nullptr), 
    m_numMeshes(0) {

    m_scene = m_importer.ReadFile(
        filePath, 
        aiProcess_CalcTangentSpace       |
        aiProcess_Triangulate            |
        aiProcess_JoinIdenticalVertices  |
        aiProcess_SortByPType
    );

    if(m_scene == nullptr || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) {
        // handle error
        std::cerr << "ERROR::ASSIMP:: " << m_importer.GetErrorString() << std::endl;
        m_scene = nullptr;
        return;
    }

    m_rootNode = m_scene->mRootNode;
    m_numMeshes = m_scene->mNumMeshes;

    processMeshes();

    std::cout << "Successfully loaded: " << m_filePath << " with " << m_numMeshes << " meshes." << std::endl;
}

void Model::processMeshes() {
    for(unsigned int i = 0; i < m_numMeshes; i++) {
        aiMesh* mesh = m_scene->mMeshes[i];

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        for(unsigned int v = 0; v < mesh->mNumVertices; v++) {
            Vertex vertex;
            vertex.position = glm::vec3(
                mesh->mVertices[v].x,
                mesh->mVertices[v].y,
                mesh->mVertices[v].z
            );

            if(mesh->HasNormals()) {
                vertex.normal = glm::vec3(
                    mesh->mNormals[v].x,
                    mesh->mNormals[v].y,
                    mesh->mNormals[v].z
                );
            }
            else {
                vertex.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            if(mesh->mTextureCoords[0]) {
                vertex.texCoords = glm::vec2(
                    mesh->mTextureCoords[0][v].x,
                    mesh->mTextureCoords[0][v].y
                );
            } else {
                vertex.texCoords = glm::vec2(0.0f, 0.0f);
            }
            vertices.push_back(vertex);
        }

        for(unsigned int f = 0; f < mesh->mNumFaces; f++) {
            aiFace face = mesh->mFaces[f];
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        m_meshes.emplace_back(vertices, indices);
    }
}

void Model::drawModel()  {
    for(auto& mesh : m_meshes) {
        mesh.drawMesh();
    }
}


Model::~Model() {
    // Assimp's Importer automatically cleans up the scene
}
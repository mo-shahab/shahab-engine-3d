#include "scene/Model.h"

// implementing the constructor
Model::Model(const std::string& filePath)
: 
    m_filePath(filePath),
    m_scene(nullptr),
    m_position(glm::vec3(0.0f)),
    m_rotation(glm::vec3(0.0f)),
    m_scale(glm::vec3(1.0f)),
    m_rootNode(nullptr), 
    m_numMeshes(0) 
{
    size_t lastSlash = filePath.find_last_of("/\\");
    std::string directory = (lastSlash == std::string::npos) ? "." : filePath.substr(0, lastSlash);

    std::cout << "[Debug] Model directory set to: " << directory << std::endl;
    m_textureLoader.m_directory = directory;

    loadModel(filePath);
    if (m_scene == nullptr) {
        std::cerr << "Failed to load model: " << filePath << std::endl;
        return;
    }

    processMeshes();
    std::cout << "Successfully loaded: " << m_filePath << " with " << m_numMeshes << " meshes." << std::endl;
}

void Model::loadModel(const std::string& path) {

    unsigned int flags = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices;

    std::string ext = path.substr(path.find_last_of('.') + 1); // substring after last dot
    for(auto& c: ext) c = static_cast<char>(std::tolower(c)); // convert to lower case

    if(ext == "obj") {
        flags |= aiProcess_FlipUVs; // Example: flip UVs for OBJ files
    } 
    else if (ext == "fbx") {
        flags |= aiProcess_GenNormals; // Example: generate normals for FBX files
    }
    
    m_scene = m_importer.ReadFile(
        path, 
        flags | aiProcess_CalcTangentSpace       | aiProcess_LimitBoneWeights        |
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
}

void Model::processMeshes() {
    for(unsigned int i = 0; i < m_numMeshes; i++) {
        aiMesh* mesh = m_scene->mMeshes[i];
        // mesh->m_isVisible = true; 

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        std::string meshName = mesh->mName.C_Str();
        std::cout << "[Debug] Processing mesh: " << meshName << " with " << mesh->mNumVertices << " vertices and " << mesh->mNumFaces << " faces." << std::endl;

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

        std::cout << "Mesh " << i << " material index: " << mesh->mMaterialIndex << std::endl;


        std::vector<Texture> textures;
        aiColor4D diffuseColor(1.0f, 1.0f, 1.0f, 1.0f);

        //loading the textures for the mesh
        if(mesh->mMaterialIndex >= 0) {
            aiMaterial* material = m_scene->mMaterials[mesh->mMaterialIndex];
            material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);

            printAllTextureTypes(material);

            // DEBUG: Print counts for common types
            int dCount = material->GetTextureCount(aiTextureType_DIFFUSE);
            int aCount = material->GetTextureCount(aiTextureType_AMBIENT);
            int bCount = material->GetTextureCount(aiTextureType_BASE_COLOR); // If using Assimp 5.1+

            if(dCount > 0 || aCount > 0) {
                std::cout << "Material " << mesh->mMaterialIndex << " has " << dCount << " diffuse and " << aCount << " ambient textures." << std::endl;
            }

            // Try loading Ambient if Diffuse is 0
            std::vector<Texture> diffuseMaps = m_textureLoader.loadTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
            if(diffuseMaps.empty())
                diffuseMaps = m_textureLoader.loadTextures(material, aiTextureType_BASE_COLOR, "texture_diffuse");
            if(diffuseMaps.empty())
                diffuseMaps = m_textureLoader.loadTextures(material, aiTextureType_AMBIENT, "texture_diffuse");
            if(diffuseMaps.empty())
                diffuseMaps = m_textureLoader.loadTextures(material, aiTextureType_UNKNOWN, "texture_diffuse");
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture> specularMaps = m_textureLoader.loadTextures(material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

            // normal maps 
            std::vector<Texture> normalMaps = m_textureLoader.loadTextures(material, aiTextureType_HEIGHT, "texture_normal");
            if(normalMaps.empty()) {
                normalMaps = m_textureLoader.loadTextures(material, aiTextureType_NORMALS, "texture_normal");
            }
            textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        }


        glm::vec4 baseColor(diffuseColor.r, diffuseColor.g, diffuseColor.b, diffuseColor.a);

        if (diffuseColor.r == 0 && diffuseColor.g == 0 && diffuseColor.b == 0) {
            // If the material color is pitch black, default to a light grey 
            // so we can actually see the model.
            baseColor = glm::vec4(0.6f, 0.6f, 0.6f, 1.0f);
        }

        for(unsigned int f = 0; f < mesh->mNumFaces; f++) {
            aiFace face = mesh->mFaces[f];
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        m_meshes.emplace_back(vertices, indices, textures, baseColor, meshName);
    }
}

void Model::draw(Shader& shader) {

    glm::mat4 transform = glm::mat4(1.0f);
    
    // transformations
    transform = glm::translate(transform, m_position);


    // rotations
    transform = glm::rotate(transform, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    
    // scaling
    transform = glm::scale(transform, glm::vec3(m_scale));
    
    shader.setMat4("u_Model", transform);

    drawModel(shader);
}

void Model::drawModel(Shader& shader)  {
    for(auto& mesh : m_meshes) {
        // this is for that check box list and stuff so yeah
        if(mesh.m_isVisible) {
            mesh.drawMesh(shader);
        }
    }
}

std::string Model::getName() const {
    // Extract the file name from the file path
    size_t lastSlash = m_filePath.find_last_of("/\\");
    size_t lastDot = m_filePath.find_last_of(".");

    if (lastDot == std::string::npos || lastDot < lastSlash) {
        lastDot = m_filePath.length(); // No extension found
    }

    return m_filePath.substr(lastSlash + 1, lastDot - lastSlash - 1);
}

glm::mat4 Model::getModelMatrix() const {
    glm::mat4 model = glm::mat4(1.0f);
    // 1. Translation
    model = glm::translate(model, m_position);
    // 2. Rotation (Order: Y -> X -> Z is common)
    model = glm::rotate(model, glm::radians(m_rotation.x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(m_rotation.y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(m_rotation.z), glm::vec3(0, 0, 1));
    // 3. Scale
    model = glm::scale(model, m_scale);
    return model;
}

Model::~Model() {
    // Assimp's Importer automatically cleans up the scene
}

// for debugging texture types
void printAllTextureTypes(aiMaterial* material) {
    std::cout << "hello" << std::endl;
    for (int t = aiTextureType_NONE; t <= aiTextureType_UNKNOWN; ++t) {
        int count = material->GetTextureCount((aiTextureType)t);
        if (count > 0) {
            std::cout << "  Texture type " << t << " has " << count << " textures." << std::endl;
        }
    }
}
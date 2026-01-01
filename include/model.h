#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <string>
#include <vector>

#include "Mesh.h"

class Model {
    public:
        // filePath is the path to the 3D model file
        const aiScene* m_scene;
        aiNode* m_rootNode;
        unsigned int m_numMeshes;

        Model(const std::string& filePath);

        void drawModel();
        ~Model();

    private: 
        std::string m_filePath;
        Assimp::Importer m_importer;
        std::vector<Mesh> m_meshes;

        void processMeshes();
};

#endif // MODEL_H
#ifndef TEXTURE_LOADER_H
#define TEXTURE_LOADER_H

#include <string>
#include <vector>
#include <assimp/material.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

// in the vendor directory
#include "stb_image.h"

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class TextureLoader {
public:
    std::string m_directory;

    std::vector<Texture> loadTextures(aiMaterial* material, aiTextureType type, std::string typeName);
private:

    std::vector<Texture> m_texturesLoaded; // to avoid loading duplicate textures
    unsigned int loadTextureFromFile(const std::string& path, const std::string& directory);
};

//helper
unsigned int loadCubemap(const std::vector<std::string>& faces);

#endif // TEXTURE_H
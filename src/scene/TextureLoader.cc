#include "scene/TextureLoader.h"

std::vector<Texture> TextureLoader::loadTextures(aiMaterial* material, aiTextureType type, std::string typeName)  {
    std::vector<Texture> textures;
    std::cout << "[Debug] Requested to load textures of type: " << typeName << std::endl;
    std::cout << "[Debug] Material has " << material->GetTextureCount(type) << " textures of type " << typeName << std::endl;

    for(unsigned int i = 0; i < material->GetTextureCount(type); i++) {
        aiString aiPath;

        material->GetTexture(type, i, &aiPath);

        std::string str(aiPath.C_Str()); // typecast aiString to std::string

        bool skip = false;

        // caching textures, if teh texture is already loaded, don't load it again
        for(unsigned int j =  0; j < m_texturesLoaded.size(); j++) {
            if(std::strcmp(m_texturesLoaded[j].path.data(), str.c_str()) == 0)
            {
                textures.push_back(m_texturesLoaded[j]);
                skip = true;
                break;
            }
        }

        if(!skip) {
            std::cout << "is this even working" << std::endl;
            Texture texture;
            texture.id = loadTextureFromFile(str, m_directory);
            texture.type = typeName;
            texture.path = str;
            textures.push_back(texture);

            m_texturesLoaded.push_back(texture); // add to loaded textures
        }
    }

    return textures;
}

unsigned int TextureLoader::loadTextureFromFile(const std::string& path, const std::string& directory) {
    std::string filename = directory + '/' + path;


    std::cout << "[Debug] Loading texture at path: " << filename << std::endl;

    unsigned int textureID;
    glGenTextures(1, &textureID); // using opengl to generate texture

    // load image using stb_image
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data) {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        stbi_image_free(data);

        return textureID;
    } 

    std::cerr << "Texture failed to load at path: " << filename << std::endl;
    stbi_image_free(data);
        
    return 0; // if the image loading failed return 0
}

unsigned int loadCubemap(const std::vector<std::string>& faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        // stbi_set_flip_vertically_on_load(false); // Cubemaps usually don't need flipping
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            // GL_TEXTURE_CUBE_MAP_POSITIVE_X is the first face, the others follow in order
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    // Set wrapping and filtering
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}
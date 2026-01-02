#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <glad/gl.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <sstream>


class Shader {
public:
    // program id for the shaders
    unsigned int m_ID;

    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    //activating the shader
    void use();

    //utility functions to set uniform variables in the shader (needed for camera)
    void setBool(const std::string &name, bool value) const;
    void setInt(const std::string &name, int value) const;
    void setFloat(const std::string &name, float value) const;

    void setMat4(const std::string &name, const glm::mat4 &mat) const;

private:
    void checkCompileErrors(unsigned int shader, std::string type);
};

#endif // SHADERS_H
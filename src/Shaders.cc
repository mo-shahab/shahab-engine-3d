#include "Shaders.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {

    // placeholders for the shaders cod3
    std::string vertexCode, fragmentCode;

    // file streams, to open the shader files
    std::ifstream vshaderFile, fshaderFile;

    vshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fshaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vshaderFile.open(vertexPath);
        fshaderFile.open(fragmentPath);

        std::stringstream vshaderStream, fshaderStream;

        // standard input the file's buffer contents into streams
        vshaderStream << vshaderFile.rdbuf();
        fshaderStream << fshaderFile.rdbuf();

        // convert stream into string
        vertexCode = vshaderStream.str();
        fragmentCode = fshaderStream.str();

        // close the files
        vshaderFile.close();
        fshaderFile.close();
    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }

    const char* vshaderCode = vertexCode.c_str();
    const char* fshaderCode = fragmentCode.c_str();

    // using unsigned int instead of uint32_t for OpenGL compatibility
    // vertex and fragment shader IDs
    unsigned int vertex, fragment;

    // creating vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vshaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");

    //creating fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fshaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");

    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");

    // delete the shaders, once linked they are no longer needed
    glDeleteShader(vertex);
    glDeleteShader(fragment);

}

void Shader::use() {
    glUseProgram(m_ID);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];

    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } 
}
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"

using namespace render;

Shader::Shader(const char* vPath, const char* fPath) {
    std::ifstream vShaderFile, fShaderFile;
    std::string vStrCode, fStrCode;
    
    vShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fShaderFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    
    try {
        vShaderFile.open(vPath);
        fShaderFile.open(fPath);
        
        std::stringstream vStream, fStream;
        vStream << vShaderFile.rdbuf();
        fStream << fShaderFile.rdbuf();
        
        vShaderFile.close();
        fShaderFile.close();
        
        // for some reason vStream.str().c_str() DOESN'T WORK
        vStrCode = vStream.str();
        fStrCode = fStream.str();
    } catch (std::ifstream::failure e) {
        std::cout << "[ERROR] Failed to load shader " << strerror(errno) << std::endl;
    }
    
    unsigned int vShader = CompileShader(vStrCode.c_str(), GL_VERTEX_SHADER);
    unsigned int fShader = CompileShader(fStrCode.c_str(), GL_FRAGMENT_SHADER);
    ID = CompileProgram(vShader, fShader);
    
    glDeleteShader(vShader);
    glDeleteShader(fShader);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetValue(const char* name, float value) {
    glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::SetValue(const char* name, int value) {
    glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::SetValue(const char* name, const glm::mat4& value) {
    glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::SetValue(const char* name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}

unsigned int Shader::CompileShader(const char* src, const GLint type) {
    unsigned int id = glCreateShader(type);
    int success;
    char log[512];
    
    glShaderSource(id, 1, &src, NULL);
    glCompileShader(id);
    
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    
    if (!success) {
        glGetShaderInfoLog(id, 512, NULL, log);
        std::cout << log << std::endl;
    }
    
    return id;
}

unsigned int Shader::CompileProgram(unsigned int vShader, unsigned int fShader) {
    unsigned int id = glCreateProgram();
    int success;
    char log[512];
    
    glAttachShader(id, vShader);
    glAttachShader(id, fShader);
    
    glLinkProgram(id);
    
    glGetProgramiv(id, GL_LINK_STATUS, &success);
    
    if (!success) {
        glGetProgramInfoLog(id, 512, NULL, log);
        std::cout << log << std::endl;
    }
    
    return id;
}

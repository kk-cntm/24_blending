#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace render {

class Shader {
public:
    unsigned int ID;
    
    Shader(const char* vPath, const char* fPath);
    void Use();
    void SetValue(const char* name, float value);
    void SetValue(const char* name, int value);
    void SetValue(const char* name, const glm::mat4& value);
    void SetValue(const char* name, const glm::vec3& value);
    
private:
    unsigned int CompileShader(const char* src, const GLint type);
    unsigned int CompileProgram(unsigned int vShader, unsigned int fShader);
};

}

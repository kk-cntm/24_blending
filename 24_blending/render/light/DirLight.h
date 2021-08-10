#pragma once

#include <glm/glm.hpp>
#include "../Shader.h"

namespace render::light {

class DirLight {
public:
    DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec);
    void Set(Shader& shader);
private:
    glm::vec3 m_direction;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
};

}

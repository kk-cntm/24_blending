#pragma once

#include <glm/glm.hpp>

namespace render {

struct Material {
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    
    static Material WhiteLight();
};

}

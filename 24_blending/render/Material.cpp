#include <glm/glm.hpp>
#include "Material.h"

using namespace render;

Material Material::WhiteLight() {
    return { glm::vec3(1.0f), glm::vec3(1.0f), 0.0f };
}

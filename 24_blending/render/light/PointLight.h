#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Attenuation.h"
#include "Shader.h"
#include "IDrawable.h"

namespace render::light {

class PointLight: IDrawable {
public:
    glm::vec3 m_position;
    
    PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, Attenuation::Distance distance);
    PointLight(glm::vec3 pos, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, Attenuation::Distance distance, std::shared_ptr<IDrawable> model);
    void Set(Shader& shader, int index);
    void Draw(Shader& shader) const;
private:
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    Attenuation m_attenuation;
    std::shared_ptr<IDrawable> m_model;
};

}

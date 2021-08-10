#pragma once

#include <glm/glm.hpp>
#include <memory>
#include "Attenuation.h"
#include "Shader.h"
#include "IDrawable.h"

namespace render::light {

class SpotLight: IDrawable {
public:
    SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance);
    SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance, std::shared_ptr<IDrawable> model);
    void Set(Shader& shader, int index);
    void Draw(Shader& shader) const;
    void SetDirection(glm::vec3 direction);
    void SetPosition(glm::vec3 position);
private:
    glm::vec3 m_position;
    glm::vec3 m_direction;
    glm::vec3 m_ambient;
    glm::vec3 m_diffuse;
    glm::vec3 m_specular;
    float m_innerConeCos;
    float m_outerConeCos;
    light::Attenuation m_attenuation;
    std::shared_ptr<IDrawable> m_model;
};

}

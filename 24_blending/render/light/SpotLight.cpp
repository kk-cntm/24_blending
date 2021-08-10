#include <string>
#include <utility>
#include "SpotLight.h"

using namespace render::light;

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance):
    m_position(pos),
    m_direction(dir),
    m_ambient(amb),
    m_diffuse(diff),
    m_specular(spec),
    m_innerConeCos(glm::cos(glm::radians(innerCone))),
    m_outerConeCos(glm::cos(glm::radians(outerCone))),
    m_attenuation(light::Attenuation::FromDistance(distance))
{}

SpotLight::SpotLight(glm::vec3 pos, glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float innerCone, float outerCone, light::Attenuation::Distance distance, std::shared_ptr<IDrawable> model):
    SpotLight(pos, dir, amb, diff, spec, innerCone, outerCone, distance)
{
    m_model = model;
}

void SpotLight::Set(Shader& shader, int index) {
    const std::string base = "spotLights[" + std::to_string(index) + "].";
    
    shader.SetValue((base + "position").c_str(), m_position);
    shader.SetValue((base + "direction").c_str(), m_direction);
    shader.SetValue((base + "ambient").c_str(), m_ambient);
    shader.SetValue((base + "diffuse").c_str(), m_diffuse);
    shader.SetValue((base + "specular").c_str(), m_specular);
    shader.SetValue((base + "constant").c_str(), m_attenuation.constant);
    shader.SetValue((base + "linear").c_str(), m_attenuation.linear);
    shader.SetValue((base + "quadratic").c_str(), m_attenuation.quadratic);
    shader.SetValue((base + "innerCone").c_str(), m_innerConeCos);
    shader.SetValue((base + "outerCone").c_str(), m_outerConeCos);
}

void SpotLight::Draw(Shader& shader) const {
    if (static_cast<bool>(m_model)) m_model->Draw(shader);
}

void SpotLight::SetDirection(glm::vec3 direction) {
    m_direction = direction;
}

void SpotLight::SetPosition(glm::vec3 position) {
    m_position = position;
}

#include "DirLight.h"

using namespace render::light;

DirLight::DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec):
    m_direction(dir),
    m_ambient(amb),
    m_diffuse(diff),
    m_specular(spec)
{}

void DirLight::Set(Shader& shader) {
    shader.SetValue("dirLight.direction", m_direction);
    shader.SetValue("dirLight.ambient", m_ambient);
    shader.SetValue("dirLight.diffuse", m_diffuse);
    shader.SetValue("dirLight.specular", m_specular);
}

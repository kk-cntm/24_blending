#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glad/glad.h>
#include "Cuboid.h"

using namespace render::shape;

Cuboid::Cuboid(glm::vec3 position, Material mat):
    m_position(position),
    m_material(mat),
    m_size(glm::vec3(1.0f)),
    m_matType(Cuboid::MaterialType::simple)
{
    Init();
}

Cuboid::Cuboid(glm::vec3 position, const std::vector<Texture>& textures):
    m_position(position),
    m_textures(textures),
    m_size(glm::vec3(1.0f)),
    m_matType(Cuboid::MaterialType::texture)
{
    Init();
}

void Cuboid::Init() {
    if (s_instances == 0) Setup();
    s_instances++;
}

void Cuboid::Draw(Shader& shader) const {
    shader.Use();
    glm::mat4 model(1.0f);
    model = glm::translate(model, m_position);
    model = glm::scale(model, m_size);
    
    shader.SetValue("model", model);
    
    switch (m_matType) {
    case Cuboid::MaterialType::texture:
        DrawWithTexture(shader);
        break;
    case Cuboid::MaterialType::simple:
        DrawWithSimple(shader);
        break;
    default: std::cerr << "[ERROR] Cuboid::Draw invalid m_matType\n";
    }
    
    glBindVertexArray(s_vao);
    glDrawArrays(GL_TRIANGLES, 0, s_vertices.size());
}

void Cuboid::Draw(Shader& shader, Shader& outlineShader) {
    shader.Use();
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT);
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    
    Draw(shader);
    
    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    
    outlineShader.Use();
    
    if (m_outline.has_value()) outlineShader.SetValue("color", m_outline.value());
    
    glm::vec3 originSize = m_size;
    m_size *= 1.01f;
    
    Draw(outlineShader);
    
    m_size = originSize;
    
    glStencilMask(0xFF);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_STENCIL_TEST);
    glDisable(GL_CULL_FACE);
}

inline void Cuboid::DrawWithSimple(Shader& shader) const {
    shader.SetValue("matrial.diffuse", m_material.diffuse);
    shader.SetValue("matrial.specular", m_material.specular);
    shader.SetValue("matrial.shininess", m_material.shininess);
}

inline void Cuboid::DrawWithTexture(Shader& shader) const {
    unsigned int diffNr = 1;
    unsigned int specNr = 1;
    
    for (int i = 0; i < m_textures.size(); i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        
        std::string name;
        
        switch (m_textures[i].type) {
        case render::Texture::diffuse:
            name = "texture_diffuse" + std::to_string(diffNr);
            diffNr++;
            break;
        case render::Texture::specular:
            name = "texture_specular" + std::to_string(specNr);
            specNr++;
            break;
        default: std::cerr << "[ERROR] Cuboid::DrawWithTexture undefined texture type\n";
        }
        
        shader.SetValue(("material." + name).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i].id);
    }
}

void Cuboid::Setup() {
    glGenVertexArrays(1, &s_vao);
    glGenBuffers(1, &s_vbo);
    
    glBindVertexArray(s_vao);
    glBindBuffer(GL_ARRAY_BUFFER, s_vbo);
    
    glBufferData(GL_ARRAY_BUFFER, s_vertices.size() * sizeof(Vertex), &s_vertices[0], GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
    glEnableVertexAttribArray(2);
    
    glBindVertexArray(0);
}

void Cuboid::SetSize(glm::vec3 size) {
    m_size = size;
}

glm::vec3 Cuboid::GetSize() const {
    return m_size;
}

void Cuboid::SetOutline(std::optional<glm::vec3> outline) {
    m_outline = outline;
}

std::optional<glm::vec3> Cuboid::GetOutline() const {
    return m_outline;
}

int Cuboid::s_instances = 0;
unsigned int Cuboid::s_vao;
unsigned int Cuboid::s_vbo;

const std::array<render::Vertex, 288> Cuboid::s_vertices = {
    // positions                    // normals                      // texture coords
    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec2(0.0f, 1.0f) }),

    Vertex({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec2(0.0f, 0.0f) }),

    Vertex({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),

    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec2(0.0f, 0.0f) }),

    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),

    Vertex({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 1.0f) }),
    Vertex({ glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(1.0f, 0.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 1.0f) }),
    Vertex({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec2(0.0f, 0.0f) })
};

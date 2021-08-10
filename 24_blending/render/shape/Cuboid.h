#pragma once

#include <glm/glm.hpp>
#include <array>
#include <optional>
#include <vector>
#include "../Texture.h"
#include "../Shader.h"
#include "../Vertex.h"
#include "../IDrawable.h"
#include "../Material.h"

namespace render::shape {

class Cuboid: public IDrawable {
public:
    Cuboid(glm::vec3 position, Material mat);
    Cuboid(glm::vec3 position, const std::vector<Texture>& textures);
    void Draw(Shader& shader) const;
    void Draw(Shader& shader, Shader& outlineShader);
    
    void SetSize(glm::vec3 size);
    glm::vec3 GetSize() const;
    
    void SetOutline(std::optional<glm::vec3> outline);
    std::optional<glm::vec3> GetOutline() const;
private:
    enum class MaterialType { texture, simple };
    
    static const std::array<Vertex, 288> s_vertices;
    static int s_instances;
    static unsigned int s_vao;
    static unsigned int s_vbo;
    
    static void Init();
    static void Setup();
    
    glm::vec3 m_size;
    glm::vec3 m_position;
    Material m_material;
    std::vector<Texture> m_textures;
    MaterialType m_matType;
    std::optional<glm::vec3> m_outline;
    
    inline void DrawWithSimple(Shader& shader) const;
    inline void DrawWithTexture(Shader& shader) const;
};

}

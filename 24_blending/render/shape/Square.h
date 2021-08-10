#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "../Vertex.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../IDrawable.h"
#include "../Material.h"

namespace render::shape {

class Square: public render::IDrawable {
public:
    Square(glm::vec3 position, Material material);
    Square(glm::vec3 position, const std::vector<Texture>& textures);
    void Draw(Shader& shader) const;
    void SetSize(glm::vec3 size);
private:
    enum class MaterialType { texture, simple };
    
    static const std::array<unsigned int, 6> s_indices;
    static unsigned int s_vao, s_vbo, s_ebo;
    static const std::array<Vertex, 4> s_vertices;
    static int s_instances;
    
    static void Setup();
    static void Init();
    
    glm::vec3 m_position;
    glm::vec3 m_size;
    MaterialType m_matType;
    Material m_material;
    std::vector<Texture> m_textures;
    
    inline void DrawWithSimple(Shader& shader) const;
    inline void DrawWithTexture(Shader& shader) const;
};

}


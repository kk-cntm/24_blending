#include <glm/glm.hpp>
#include <vector>
#include <array>
#include "../Vertex.h"
#include "../Texture.h"
#include "../Shader.h"
#include "../IDrawable.h"
#include "../Material.h"

namespace render::shape {

class Plane: public render::IDrawable {
public:
    struct TextureParams {
        float texWrapS;
        float texWrapT;
    };
    
    static constexpr TextureParams s_defaultTexParams = { 1, 1 };
    
    Plane(glm::vec3 position, Material material);
    Plane(glm::vec3 position, const std::vector<Texture>& textures, TextureParams initParams = s_defaultTexParams);
    void Draw(Shader& shader) const;
    void SetSize(glm::vec3 size);
private:
    enum class MaterialType { texture, simple };
    
    static const std::array<unsigned int, 6> s_indices;
    static unsigned int s_ebo;
    
    static std::array<Vertex, 4> CalcVertices(TextureParams params);
    
    glm::vec3 m_position;
    glm::vec3 m_size;
    MaterialType m_matType;
    Material m_material;
    std::vector<Texture> m_textures;
    unsigned int m_vao, m_vbo;
    std::array<Vertex, 4> m_vertices;
    
    void Setup();
    
    inline void DrawWithSimple(Shader& shader) const;
    inline void DrawWithTexture(Shader& shader) const;
};

}

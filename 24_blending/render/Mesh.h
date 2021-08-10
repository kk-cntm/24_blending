#pragma once

#include <vector>
#include "Vertex.h"
#include "Texture.h"
#include "Shader.h"

namespace render {

class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
    
    void Draw(Shader& shader);
    
private:
    unsigned int m_vao, m_vbo, m_ebo;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
    
    void SetupMesh();
};

}

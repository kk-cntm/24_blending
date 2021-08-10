#pragma once

#include <vector>
#include <string>
#include <assimp/scene.h>
#include "Mesh.h"
#include "Shader.h"

namespace render {

class Model {
public:
    Model(const char* path);
    void Draw(Shader& shader);
private:
    static unsigned int LoadTexture(const std::string& path);
    
    std::vector<Texture> m_loadedTextures;
    std::vector<Mesh> m_meshes;
    std::string m_directory;
    void LoadModel(std::string path);
    void ProcessNode(aiNode* node, const aiScene* scene);
    Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
    std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, Texture::Type type);
};

}

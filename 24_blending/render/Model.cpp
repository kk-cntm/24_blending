#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <algorithm>
#include <functional>
#include "Model.h"

using namespace render;

Model::Model(const char* path) {
    LoadModel(path);
}

void Model::Draw(Shader& shader) {
    for (int i = 0; i < m_meshes.size(); i++) {
        m_meshes[i].Draw(shader);
    }
}

void Model::LoadModel(std::string path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
    
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "[ERROR] Model LoadModel" << importer.GetErrorString() << std::endl;
        exit(1);
    }
    
    m_directory = path.substr(0, path.find_last_of('/'));
    
    ProcessNode(scene->mRootNode, scene);
}

void Model::ProcessNode(aiNode* node, const aiScene* scene) {
    for (int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        m_meshes.push_back(ProcessMesh(mesh, scene));
    }
    
    for (int i = 0; i < node->mNumChildren; i++) {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    
    for (int i = 0; i < mesh->mNumVertices; i++) {
        glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        glm::vec2 texCoords;
        
        if (mesh->mTextureCoords[0]) {
            texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        } else {
            texCoords = glm::vec2(0.0f, 0.0f);
        }
        
        Vertex vertex;
        vertex.position = position;
        vertex.normal = normal;
        vertex.texCoords = texCoords;
        
        vertices.push_back(vertex);
    }
    
    for (int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    if (mesh->mMaterialIndex >= 0) {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        
        std::vector<Texture> diffuseTextures = LoadMaterialTextures(material, Texture::diffuse);
        textures.insert(textures.end(), diffuseTextures.begin(), diffuseTextures.end());
        
        std::vector<Texture> specularTextures = LoadMaterialTextures(material, Texture::specular);
        textures.insert(textures.end(), specularTextures.begin(), specularTextures.end());
    }
    
    return Mesh(vertices, indices, textures);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, Texture::Type type) {
    std::vector<Texture> textures;
    
    aiTextureType texType = Texture::GetAssimpType(type);
    
    for (int i = 0; i < mat->GetTextureCount(texType); i++) {
        aiString str;
        mat->GetTexture(texType, i, &str);
        std::string path = std::string(str.C_Str());
        
        auto loadedTexture = std::find_if(
            m_loadedTextures.cbegin(),
            m_loadedTextures.cend(),
            [&path](const Texture& tex) { return std::strcmp(tex.path.c_str(), path.c_str()); }
        );
        
        if (loadedTexture != m_loadedTextures.end()) {
            textures.push_back(*loadedTexture);
            continue;
        };
        
        Texture texture;
        texture.type = type;
        texture.id = Model::LoadTexture(m_directory + '/' + path);
        texture.path = path;
        
        m_loadedTextures.push_back(texture);
        textures.push_back(texture);
    }
    
    return textures;
}

unsigned int Model::LoadTexture(const std::string& path) {
    unsigned int id;
    
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
    
    if (data) {
        GLenum format;
        
        if (channels == 1) format = GL_RED;
        else if (channels == 4) format = GL_RGBA;
        else format = GL_RGB;
        
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "[ERROR] Failed to load texture with path: " << path << std::endl;
    }
    
    stbi_image_free(data);
    
    return id;
}

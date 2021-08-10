#include <iostream>
#include <stb/stb_image.h>
#include <glad/glad.h>
#include "Texture.h"

using namespace render;

std::map<std::string, Texture> Texture::s_loadedTextures;

Texture Texture::Load(const char* path, Texture::Type type, Texture::LoadParams params) {
    auto existing = s_loadedTextures.find(path);
    
    if (existing != s_loadedTextures.end()) {
        Texture texture = existing->second;
        texture.type = type;
        
        return texture;
    }
    
    Texture texture;
    texture.path = path;
    texture.type = type;
    
    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, params.wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    
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
    
    return texture;
}

aiTextureType Texture::GetAssimpType(Texture::Type type) {
    switch (type) {
        case Texture::diffuse: return aiTextureType_DIFFUSE;
        case Texture::specular: return aiTextureType_SPECULAR;
        default: std::cerr << "[ERROR] Texture::GetAssimpType invalid type\n";
    }
}

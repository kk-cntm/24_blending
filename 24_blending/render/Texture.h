#pragma once

#include <string>
#include <map>
#include "glad/glad.h"
#include <assimp/scene.h>

namespace render {

struct Texture {
    enum Type { diffuse, specular };
    struct LoadParams {
        GLenum wrapS;
        GLenum wrapT;
    };
    
    unsigned int id;
    Type type;
    std::string path;
    
    static Texture Load(const char* path, Type type, LoadParams params = defaultLoadParams);
    static aiTextureType GetAssimpType(Texture::Type type);
    
private:
    static std::map<std::string, Texture> s_loadedTextures;
    static constexpr LoadParams defaultLoadParams = { GL_REPEAT, GL_REPEAT };
};

}

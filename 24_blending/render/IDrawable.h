#pragma once

#include "Shader.h"

namespace render {

class IDrawable {
public:
    virtual void Draw(Shader& shader) const = 0;
};

}

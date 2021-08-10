#version 330 core

struct Material {
    sampler2D texture_diffuse1;
};

out vec4 FragColor;

in vec2 TexPos;

uniform Material material;

void main() {
    FragColor = texture(material.texture_diffuse1, TexPos);
}

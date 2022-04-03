#version 410 core

struct Material {
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

in vec3 normal;

out vec4 fragColor;

uniform Material material;

void main() {
    fragColor = vec4(material.diffuse, 1.0);
}
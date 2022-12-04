#version 410 core

struct Material {
    vec3 diffuse;
    vec3 specular;
    int shininess;

    bool useVertexColor;
};

in vec3 normal;
in vec3 color;

out vec4 fragColor;

uniform Material material;

void main() {

    vec3 lightDir = normalize(vec3(0.3, -1.0, 0.2));
    float att = dot(normal, -lightDir);
    att = clamp(att, 0.3, 1.0);
    att = 1.0;

//    fragColor = vec4(0.0, 1.0, 0.0, 1.0);

    if(material.useVertexColor)
      fragColor = vec4(color * att, 1.0);
//      fragColor = vec4(1.0, 0.0, 0.0, 1.0);
    else
      fragColor = vec4(material.diffuse * att, 1.0);

//    fragColor = vec4(0.0, 0.0, 1.0, 1.0);
}

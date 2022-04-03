#version 410 core

// Structures
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    int shininess;
    sampler2D diffuse;
    sampler2D specular;
};

// IO
in vec3 fragPos;
in vec3 normal;
in vec2 texCoords;

out vec4 fragColor;


// Uniforms
// Camera
uniform vec3 viewPos;

// Lights
uniform PointLight pointLight;

// Objects
uniform Material material;

void main() {

    vec3 normal = gl_FrontFacing ? normal : -normal;

    // ambient
    vec3 ambient = vec3(texture(material.diffuse, texCoords)) * pointLight.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, texCoords)) * diff * pointLight.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, texCoords)) * spec * pointLight.specular;


    fragColor = vec4(ambient + diffuse + specular, 1.0);
    // fragColor = texture(diffuseTexture, texCoords);
}
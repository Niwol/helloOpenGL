#version 410 core

// Structures
struct PointLight {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material {
    vec3 diffuse;
    vec3 specular;
    int shininess;
};

// IO
in vec3 fragPos;
in vec3 normal;

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
    vec3 ambient = material.diffuse * pointLight.ambient;

    // diffuse
    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(pointLight.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = material.diffuse * diff * pointLight.diffuse;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = material.specular * spec * pointLight.specular;


    vec3 result = (ambient + diffuse + specular);
    fragColor = vec4(result, 1.0);
}
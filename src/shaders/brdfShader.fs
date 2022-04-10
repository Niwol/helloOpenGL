#version 410 core

#define MAX_POINT_LIGHTS 10
#define PI 3.1415926535897932384626433832795

// Structures
struct PointLight {
    vec3 position;
};

struct Material {
    vec3 diffuse;
    float roughness;
    float metallic;
};

// IO
in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

// Uniforms
// Camera
uniform vec3 viewPos;

// Lights
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform int nbPointLights;

// Objects
uniform Material material;


// Functions
vec3 calculateLightContribution(PointLight light);
float D(float alpha, vec3 N, vec3 H);
float G(float alpha, vec3 V, vec3 N, vec3 L, vec3 H);
float chiplus(float x);


void main() {

    vec3 result = vec3(0.0);
    for(int i = 0; i < nbPointLights; i++)
        result += calculateLightContribution(pointLights[i]);

    fragColor = vec4(result, 1.0);
}


vec3 calculateLightContribution(PointLight light) {

    vec3 V = normalize(viewPos - fragPos);
    vec3 L = normalize(light.position - fragPos);
    vec3 N = normal;
    vec3 H = normalize(L + V);

    vec3 c_diff = mix(material.diffuse, vec3(0.0), material.metallic);
    vec3 f0 = mix(vec3(0.04), material.diffuse, material.metallic);
    float alpha = material.roughness * material.roughness;

    vec3 F = f0 + (1 - f0) * (1 - abs(dot(V, H)));

    vec3 f_diffuse = gl_FrontFacing ? (1 - F) * (1 / PI) * c_diff : vec3(0.0);
    vec3 f_specular = F * D(alpha, N, H) * G(alpha, V, N, L, H) / (4 * abs(dot(V, N)) * abs(dot(L, N)));

    return dot(N, L) * (f_diffuse + f_specular);
}

float D(float alpha, vec3 N, vec3 H) {

    float NdotH = dot(N, H);
    float alpha_sq = alpha * alpha;

    float nomi = alpha_sq * chiplus(NdotH);
    float denomi = PI * pow((NdotH * NdotH * (alpha_sq - 1.0) + 1), 2);

    return nomi / denomi;
}


float G(float alpha, vec3 V, vec3 N, vec3 L, vec3 H) {

    float NdotL = dot(N, L);
    float HdotL = dot(H, L);
    float NdotV = dot(N, V);
    float HdotV = dot(H, V);

    float alpha_sq = alpha * alpha;

    float nomi =      2 * abs(NdotL) * chiplus(HdotL)
                    * 2 * abs(NdotV) * chiplus(HdotV); 

    float denomi =    abs(NdotL) + sqrt(alpha_sq + (1 - alpha_sq) * pow(NdotL, 2))
                    * abs(NdotV) + sqrt(alpha_sq + (1 - alpha_sq) * pow(NdotV, 2));

    return nomi / denomi;
}

float chiplus(float x) {
    return x > 0.0 ? 1.0 : 0.0;
}
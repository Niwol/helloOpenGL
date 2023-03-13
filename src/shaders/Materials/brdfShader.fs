#version 410 core

/* ******************************************************** *
 * *                                                        *
 * *                                                        *
 * *                DIRECTIONAL_LIGHT_GLSL                  *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */

struct DirectionalLight
{
    vec3 direction;
};

float getDirectionalLightAttenuation(DirectionalLight light, vec3 fragPos)
{
    return 1.0;
}

vec3 getDirectionalLightDirection(DirectionalLight light, vec3 fragPos)
{
    return normalize(-light.direction);
}

/* ******************************************************** *
 * *                                                        *
 * *                                                        *
 * *                   POINT_LIGHT_GLSL                     *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */

struct PoinLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

float getPointLightAttenuation(PoinLight light, vec3 fragPos)
{
    float lightDist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant 
                             + light.linear * lightDist 
                             + light.quadratic * (lightDist * lightDist));

    return attenuation;
}

vec3 getPointLightDirection(PoinLight light, vec3 fragPos)
{
    return normalize(light.position - fragPos);
}

/* ******************************************************** *
 * *                                                        *
 * *                                                        *
 * *                    SPOT_LIGHT_GLSL                     *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */

struct SpotLight
{
    vec3 position;

    vec3 direction;
    float cutOffAngle;
    float outerCutOffAngle;

    float constant;
    float linear;
    float quadratic;
};

float getSpotLightAttenuation(SpotLight light, vec3 fragPos)
{
    vec3 lightDir = light.position - fragPos;
    float lightDist = length(lightDir);
    lightDir = normalize(lightDir);

    float attenuation = 1.0 / (light.constant 
                             + light.linear * lightDist 
                             + light.quadratic * (lightDist * lightDist));

    vec3 spotDir = normalize(light.direction);
    
    float cosTheta = dot(lightDir, -spotDir);
    float epsilon = light.cutOffAngle - light.outerCutOffAngle;
    float intesity = clamp((cosTheta - light.outerCutOffAngle) / epsilon, 0.0, 1.0);

    return attenuation * intesity;
}

vec3 getSpotLightDirection(SpotLight light, vec3 fragPos)
{
    return normalize(light.position - fragPos);
}

/* ******************************************************** *
 * *                                                        *
 * *                                                        *
 * *                      LIGHT_GLSL                        *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */
    
#define DIRECTION   0
#define POINT       1
#define SPOT        2


uniform int lightType;

uniform DirectionalLight directionalLight;
uniform PoinLight pointLight;
uniform SpotLight spotLight;

uniform vec3 lightColor;


vec3 getLightColor(vec3 fragPos)
{
    switch(lightType)
    {
        case DIRECTION:
            return lightColor * getDirectionalLightAttenuation(directionalLight, fragPos);
            break;

        case POINT:
            return lightColor * getPointLightAttenuation(pointLight, fragPos);
            break;

        case SPOT:
            return lightColor * getSpotLightAttenuation(spotLight, fragPos);
            break;
    }
}

vec3 getLightDirection(vec3 fragPos)
{
    switch(lightType)
    {
        case DIRECTION:
            return getDirectionalLightDirection(directionalLight, fragPos);
            break;

        case POINT:
            return getPointLightDirection(pointLight, fragPos);
            break;

        case SPOT:
            return getSpotLightDirection(spotLight, fragPos);
            break;
    }
}


/* ******************************************************** *
 * *                                                        *
 * *                                                        *
 * *                        SHADER                          *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */

#define PI 3.1415926535897932384626433832795
#define EPSYLON 1.0e-10

// Structures
struct PointLight {
    vec3 position;
};

struct Material {
    vec3 albedo;
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

// Objects
uniform Material material;


// Functions
float D(float alpha, vec3 N, vec3 H);
float G(float alpha, vec3 V, vec3 N, vec3 L, vec3 H);
float chiplus(float x);

float vis(float alpha, vec3 V, vec3 N, vec3 L, vec3 H);
vec3 specular_brdf(float alpha, vec3 V, vec3 N, vec3 L, vec3 H);
vec3 diffuse_brdf(vec3 color);

vec3 fresnel_mix(vec3 layer, vec3 base, float ior, float VdotH);
vec3 conductor_fresnel(vec3 f0, vec3 bsdf, float VdotH);

void main() 
{
    vec3 V = normalize(viewPos - fragPos);
    vec3 L = getLightDirection(fragPos);
    vec3 N = normalize(normal);
    vec3 H = normalize(L + V);

    float VdotH = dot(V, H);
    float cosTheta = max(dot(N, L), 0.0);

    vec3 c_diff = mix(material.albedo, vec3(0.0), material.metallic);
    vec3 f0 = mix(vec3(0.04), material.albedo, material.metallic);
    float alpha = material.roughness * material.roughness;

    vec3 F = f0 + (1 - f0) * (1 - abs(dot(V, H)));

    vec3 f_diffuse = gl_FrontFacing ? (1 - F) * (1 / PI) * c_diff : vec3(0.0);
    vec3 f_specular = F * D(alpha, N, H) * G(alpha, V, N, L, H) / (4 * abs(dot(V, N)) * abs(dot(L, N)));

    vec3 result = dot(N, L) * (f_diffuse + f_specular);
    result = result * getLightColor(fragPos);

    vec3 lightAtt = getLightColor(fragPos);
//
//    float alpha = material.roughness * material.roughness;
//
//    vec3 spec_brdf = specular_brdf(alpha, V, N, L, H);
//
//    vec3 layer = spec_brdf;
//    vec3 base = diffuse_brdf(material.albedo);
//
//    vec3 bsdf = spec_brdf;
//
//    vec3 bsdf0 = fresnel_mix(layer, base, 1.5, VdotH);
//    vec3 bsdf1 = conductor_fresnel(bsdf, material.albedo, VdotH);
//
//    vec3 result = mix(bsdf0, bsdf1, material.metallic);

    fragColor = vec4(result * lightAtt * cosTheta, 1.0);
}


float D(float alpha, vec3 N, vec3 H) {

    float NdotH = dot(N, H);
    float alpha_sq = alpha * alpha + EPSYLON;

    float nomi = alpha_sq * chiplus(NdotH);
    float denomi = PI * pow((NdotH * NdotH * (alpha_sq - 1.0) + 1.0), 2);

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

float vis(float alpha, vec3 V, vec3 N, vec3 L, vec3 H)
{
    float NdotL = dot(N, L);
    float HdotL = dot(H, L);
    float NdotV = dot(N, V);
    float HdotV = dot(H, V);

    float alpha_sq = alpha * alpha;

    float nomi = chiplus(HdotL) * chiplus(HdotV); 

    float denomi =    abs(NdotL) + sqrt(alpha_sq + (1 - alpha_sq) * pow(NdotL, 2))
                    * abs(NdotV) + sqrt(alpha_sq + (1 - alpha_sq) * pow(NdotV, 2));

    return nomi / denomi;
}

vec3 specular_brdf(float alpha, vec3 V, vec3 N, vec3 L, vec3 H)
{
    return material.albedo * vis(alpha, V, N, L, H) * D(alpha, N, H);
}

vec3 diffuse_brdf(vec3 color)
{
    return (1.0 / PI) * color;
}

vec3 fresnel_mix(vec3 layer, vec3 base, float ior, float VdotH)
{
    float f0 = pow(((1.0 - ior) / (1.0 + ior)), 2);
    float fr = f0 + pow((1.0 - f0) * (1.0 - abs(VdotH)), 5);
    return mix(base, layer, fr);
}

vec3 conductor_fresnel(vec3 f0, vec3 bsdf, float VdotH)
{
    return bsdf * (f0 + (1.0 - f0) * pow(1.0 - abs(VdotH), 5));
}

float chiplus(float x) 
{
    return x > 0.0 ? 1.0 : 0.0;
}

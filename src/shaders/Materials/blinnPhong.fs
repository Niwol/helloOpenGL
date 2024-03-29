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


struct Material
{
    vec3 diffuse;
    vec3 specular;
    int shininess;

    bool hasTexture;
    sampler2D diffuseTexture;
    sampler2D specularTexture;
};

// IO
in vec3 fragPos;
in vec3 normal;
in vec2 uv;

out vec4 fragColor;

// Uniforms
// Camera
uniform vec3 viewPos;

// Objects
uniform Material material;

void main() 
{
    vec3 diffuse;
    vec3 specular;
    
    if(material.hasTexture)
    {
        diffuse = vec3(texture(material.diffuseTexture, uv));
        specular = vec3(texture(material.specularTexture, uv));
    }
    else
    {
        diffuse = material.diffuse;
        specular = material.specular;
    }


    vec3 lightAtt = getLightColor(fragPos);
    vec3 lightDir = getLightDirection(fragPos);

    float cosTheta = max(dot(lightDir, normal), 0.0);

    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-viewDir, normal);
    float specularStrength = pow(max(dot(reflectDir, lightDir), 0.0),
                                 material.shininess);

    vec3 result = diffuse * cosTheta
                + specular * specularStrength;

    result = result * lightAtt;
    
    fragColor = vec4(vec3(result), 1.0);
}


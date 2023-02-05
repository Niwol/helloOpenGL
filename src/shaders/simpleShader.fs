#version 410 core


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
 * *                      LIGHT_GLSL                        *
 * *                                                        *
 * *                                                        *
 * ******************************************************** */
    
#define DIRECTION   0
#define POINT       1
#define SPOT        2


uniform int lightType;

uniform PoinLight pointLight;

uniform vec3 lightColor;


vec3 getLightColor(vec3 fragPos)
{
    switch(lightType)
    {
        case DIRECTION:
            return vec3(0.0);
            break;

        case POINT:
            return lightColor * getPointLightAttenuation(pointLight, fragPos);
            break;

        case SPOT:
            return vec3(0.0);
            break;
    }
}

vec3 getLightDirection(vec3 fragPos)
{
    switch(lightType)
    {
        case DIRECTION:
            return vec3(0.0);
            break;

        case POINT:
            return getPointLightDirection(pointLight, fragPos);
            break;

        case SPOT:
            return vec3(0.0);
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

in vec3 fragPos;
in vec3 normal;

out vec4 fragColor;

uniform vec3 color;
//uniform PointLight light;

void main() 
{
    vec3 lightAtt = getLightColor(fragPos);
    vec3 lightDir = getLightDirection(fragPos);

    vec3 ambiant = vec3(0.1) * lightColor;
    float cosTheta = max(dot(lightDir, normal), 0.0);
    
    fragColor = vec4(lightColor * color * cosTheta + ambiant, 1.0);
}

#ifndef POINT_LIGHT_GLSL
#define POINT_LIGHT_GLSL

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

#endif // POINT_LIGHT_GLSL


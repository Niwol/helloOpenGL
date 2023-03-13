#ifndef SPOT_LIGHT_GLSL
#define SPOT_LIGHT_GLSL

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

#endif // SPOT_LIGHT_GLSL



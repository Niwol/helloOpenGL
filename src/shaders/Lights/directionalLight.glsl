#ifndef DIRECTIONAL_LIGHT_GLSL
#define DIRECTIONAL_LIGHT_GLSL

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

#endif // DIRECTIONAL_LIGHT_GLSL



#ifndef POINT_LIGHT_GLSL
#define POINT_LIGHT_GLSL

struct PoinLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;
};

vec3 getPointLightColor(PoinLight light, vec3 fragPos)
{
    float lightDist = norm(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * lightDist
                             + light.quadratic * (lightDist * lightDist));

    return lightColor * attenuation;
}

vec3 getPointLightDirection(PoinLight light, vec3 fragPos)
{
    return normalize(light.position - fragPos);
}

#endif // POINT_LIGHT_GLSL


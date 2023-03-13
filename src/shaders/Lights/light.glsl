#ifndef LIGHT_GLSL
#define LIGHT_GLSL

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

#endif // LIGHT_GLSL

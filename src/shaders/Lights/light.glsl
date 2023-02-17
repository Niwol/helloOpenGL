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

#endif // LIGHT_GLSL

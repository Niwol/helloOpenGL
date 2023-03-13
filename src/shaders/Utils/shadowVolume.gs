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

layout (triangles) in;
layout (triangle_strip, max_vertices = 18) out;

in VS_OUT
{
    vec3 worldPos;
} gs_in[];

const float EPSYLON = 0.01;
const float SHADOW_SIZE = 10.0;

uniform mat4 view;
uniform mat4 projection;


void emitQuad(vec3 startVertex, vec3 endVertex, vec3 normal)
{
    vec3 lightDir = getLightDirection(startVertex);
    gl_Position = projection * view * vec4(startVertex - lightDir * EPSYLON - normal * EPSYLON, 1.0);
    EmitVertex();
    
    //gl_Position = projection * view * vec4(startVertex - lightDir * SHADOW_SIZE, 1.0);
    gl_Position = projection * view * vec4(-lightDir, 0.0);
    EmitVertex();

    lightDir = getLightDirection(endVertex);
    gl_Position = projection * view * vec4(endVertex - lightDir * EPSYLON - normal * EPSYLON, 1.0);
    EmitVertex();

    //gl_Position = projection * view * vec4(endVertex - lightDir * SHADOW_SIZE, 1.0);
    gl_Position = projection * view * vec4(-lightDir, 0.0);
    EmitVertex();
    
    EndPrimitive();
}

void main()
{
    vec3 e0 = gs_in[2].worldPos - gs_in[0].worldPos;
    vec3 e1 = gs_in[1].worldPos - gs_in[0].worldPos;

    vec3 normal = normalize(cross(e1, e0));
    vec3 lightDir = getLightDirection(gs_in[0].worldPos);

    if(dot(normal, lightDir) > 0.0)
    {
        emitQuad(gs_in[0].worldPos, gs_in[1].worldPos, normal);
        emitQuad(gs_in[1].worldPos, gs_in[2].worldPos, normal);
        emitQuad(gs_in[2].worldPos, gs_in[0].worldPos, normal);

        // Front
        gl_Position = projection * view * vec4(gs_in[0].worldPos - lightDir * EPSYLON - normal * EPSYLON, 1.0);
        EmitVertex();

        lightDir = getLightDirection(gs_in[1].worldPos);
        gl_Position = projection * view * vec4(gs_in[1].worldPos - lightDir * EPSYLON - normal * EPSYLON, 1.0);
        EmitVertex();

        lightDir = getLightDirection(gs_in[2].worldPos);
        gl_Position = projection * view * vec4(gs_in[2].worldPos - lightDir * EPSYLON - normal * EPSYLON, 1.0);
        EmitVertex();

        EndPrimitive();

        // Back
        lightDir = getLightDirection(gs_in[0].worldPos);
        //gl_Position = projection * view * vec4(gs_in[0].worldPos - lightDir * SHADOW_SIZE, 1.0);
        gl_Position = projection * view * vec4(-lightDir, 0.0);
        EmitVertex();
        
        lightDir = getLightDirection(gs_in[2].worldPos);
        //gl_Position = projection * view * vec4(gs_in[2].worldPos - lightDir * SHADOW_SIZE, 1.0);
        gl_Position = projection * view * vec4(-lightDir, 0.0);
        EmitVertex();

        lightDir = getLightDirection(gs_in[1].worldPos);
        //gl_Position = projection * view * vec4(gs_in[1].worldPos - lightDir * SHADOW_SIZE, 1.0);
        gl_Position = projection * view * vec4(-lightDir, 0.0);
        EmitVertex();

        EndPrimitive();
    }
}


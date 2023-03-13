#version 410 core

in vec2 uv;

out vec4 fragColor;

uniform vec3 lightColor;
uniform vec3 color;

uniform bool hasTexture;
uniform sampler2D tex;

const float ambientStrength = 0.01;

void main() 
{
    vec3 ambient;

    if(hasTexture)
    {
        ambient = vec3(texture(tex, uv));
    }
    else
    {
        ambient = color;
    }

    ambient *= ambientStrength;
        
    fragColor = vec4(lightColor * ambient, 1.0);
}


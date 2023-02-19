#version 410 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

uniform mat4 model;

out VS_OUT
{
    vec3 worldPos;
} vs_out;

void main() 
{
    vs_out.worldPos = vec3(model * vec4(in_position, 1.0));
}

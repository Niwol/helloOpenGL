#version 410 core
layout (location = 0) in vec3 in_position;
layout (location = 2) in vec2 in_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 uv;

void main() {
    gl_Position = projection * view * model * vec4(in_position, 1.0);

    uv = in_uv;
}
#version 410 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 2) in vec2 in_texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 fragPos;
out vec2 texCoords;

void main() {
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    // gl_Position = vec4(in_position, 1.0);

    normal = mat3(transpose(inverse(model))) * in_normal;
    fragPos = vec3(model * vec4(in_position, 1.0));
    texCoords = in_texCoords;
}
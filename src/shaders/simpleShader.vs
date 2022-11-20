#version 410 core
layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;
layout (location = 3) in vec3 in_color;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec3 color;

void main() {
    gl_Position = projection * view * model * vec4(in_position, 1.0);
    // gl_Position = vec4(in_position, 1.0);

    normal = normalize(mat3(transpose(inverse(model))) * in_normal);
    color = in_color;
}

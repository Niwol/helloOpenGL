#version 410 core

in vec3 normal;

out vec4 fragColor;

void main() {
    vec3 normal = gl_FrontFacing ? normal : -normal;
    fragColor = vec4(normal * 0.5 + 0.5, 1.0);
}
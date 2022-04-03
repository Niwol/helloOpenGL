#version 410 core

in vec3 normal;

out vec4 fragColor;

void main() {
    vec3 n = gl_FrontFacing ? normal : -normal;
    fragColor = vec4(n * 0.5 + 0.5, 1.0);
}
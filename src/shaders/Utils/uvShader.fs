#version 410 core

in vec2 uv;

out vec4 fragColor;

void main() {
    fragColor = vec4(uv.x, uv.y, 1.0 - (uv.x + uv.y) / 2.0, 1.0);
}
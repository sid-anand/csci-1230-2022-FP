#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uvIn;

out vec2 uv;

void main() {
    uv = uvIn;
    gl_Position = vec4(position, 1.0);
}

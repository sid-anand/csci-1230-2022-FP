#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvIn;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;

uniform mat4 view;
uniform mat4 proj;

void main() {
    worldPosition = position;

    worldNormal = normal;

    uv = uvIn;

    // note the lack of a model matrix because VBO positions are already in world space
    gl_Position = proj * view * vec4(position, 1.0);
}

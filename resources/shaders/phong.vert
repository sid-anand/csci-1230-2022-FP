#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uvIn;

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 uv;

//uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    worldPosition = position;

    // Recall that transforming normals requires obtaining the inverse-transpose of the model matrix!
    // In projects 5 and 6, consider the performance implications of performing this here.
    worldNormal = normal;

    uv = uvIn;

    gl_Position = proj * view * vec4(position, 1.0);
}

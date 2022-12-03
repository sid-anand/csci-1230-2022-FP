#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 worldPosition;
out vec3 worldNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() {
    worldPosition = vec3(model * vec4(position, 1.0));

    // Recall that transforming normals requires obtaining the inverse-transpose of the model matrix!
    // In projects 5 and 6, consider the performance implications of performing this here.
    worldNormal = transpose(inverse(mat3(model))) * normal;

    gl_Position = proj * view * model * vec4(position, 1.0);
}

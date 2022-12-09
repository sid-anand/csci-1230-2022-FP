#version 330 core

out vec4 fragColor;

in vec3 texCoords;

uniform samplerCube daySkybox;
uniform samplerCube nightSkybox;
uniform float skyboxBlend;

void main() {
    vec4 dayColor = texture(daySkybox, texCoords);
    vec4 nightColor = texture(nightSkybox, texCoords);
    fragColor = (1 - skyboxBlend) * dayColor + skyboxBlend * nightColor;
}

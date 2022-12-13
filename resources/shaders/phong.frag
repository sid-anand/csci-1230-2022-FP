#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec4 cameraPos;

uniform vec4 light1Direction;
uniform vec4 light2Direction;
uniform vec4 light1Color;
uniform vec4 light2Color;
uniform bool isShiny;

void main() {
    vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);
    float shininess = 100;

    vec3 normal = normalize(worldNormal);
    vec3 directionToLight1 = normalize(vec3(-light1Direction));
    vec3 directionToLight2 = normalize(vec3(-light2Direction));

    fragColor = vec4(0.0);

    // ambient term
    fragColor += ambientColor;

    // diffuse term
    float cosNormalLight1 = min(max(dot(normal, directionToLight1), 0.0), 1.0);
    float cosNormalLight2 = min(max(dot(normal, directionToLight2), 0.0), 1.0);
    vec4 textureDiffuseColor = texture(texture1, uv);
    fragColor += light1Color * textureDiffuseColor * min(max(dot(normal, directionToLight1), 0.0), 1.0);
    fragColor += light2Color * textureDiffuseColor * min(max(dot(normal, directionToLight2), 0.0), 1.0);

    // specular term
    if (isShiny) {
        vec3 reflectionDirection1 = reflect(-directionToLight1, normal);
        vec3 reflectionDirection2 = reflect(-directionToLight2, normal);
        vec3 directionToCamera = normalize(vec3(cameraPos) - worldPosition);
        if (shininess <= 0) {
            fragColor += light1Color * specularColor;
            fragColor += light2Color * specularColor;
        } else {
            float cosReflectionCamera1 = min(max(dot(reflectionDirection1, directionToCamera), 0.0), 1.0);
            float cosReflectionCamera2 = min(max(dot(reflectionDirection2, directionToCamera), 0.0), 1.0);
            fragColor += light1Color * specularColor * pow(cosReflectionCamera1, shininess);
            fragColor += light2Color * specularColor * pow(cosReflectionCamera2, shininess);
        }
    }
}

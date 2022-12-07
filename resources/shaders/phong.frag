#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D texture1;
uniform vec4 cameraPos;
uniform vec4 lightDirection;
uniform vec4 lightColor;

void main() {
    vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0);
//    vec4 diffuseColor = vec4(0.5, 0.5, 0.5, 1.0);
    vec4 specularColor = vec4(1.0, 1.0, 1.0, 1.0);
    float shininess = 100;

    vec3 normal = normalize(worldNormal);
    vec3 directionToLight = normalize(vec3(-lightDirection));

    fragColor = vec4(0.0);

    // ambient term
    fragColor += ambientColor;

    // diffuse term
    float cosNormalLight = min(max(dot(normal, directionToLight), 0.0), 1.0);
    vec4 textureDiffuseColor = texture(texture1, uv);
    fragColor += lightColor * textureDiffuseColor * min(max(dot(normal, directionToLight), 0.0), 1.0);

    // specular term
    vec3 reflectionDirection = reflect(-directionToLight, normal);
    vec3 directionToCamera = normalize(vec3(cameraPos) - worldPosition);
    if (shininess <= 0) {
        fragColor += lightColor * specularColor;
    } else {
        float cosReflectionCamera = min(max(dot(reflectionDirection, directionToCamera), 0.0), 1.0);
        fragColor += lightColor * specularColor * pow(cosReflectionCamera, shininess);
    }
}

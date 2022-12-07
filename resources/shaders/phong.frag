#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 uv;

out vec4 fragColor;

uniform sampler2D texture1;

//uniform float ka;
//uniform float kd;
//uniform float ks;

//uniform vec4 ambientColor;
//uniform vec4 diffuseColor;
//uniform vec4 specularColor;
//uniform float shininess;

//uniform vec4 cameraPos;

//struct Light {
//    int type;
//    vec4 color;
//    vec3 function;
//    vec4 pos;
//    vec4 dir;
//    float penumbra;
//    float angle;
//};

//uniform int numLights;
//uniform Light lights[8];

//vec4 spotLightIntensity(Light spotLight, vec3 position) {
//    float thetaOuter = spotLight.angle;
//    float thetaInner = spotLight.angle - spotLight.penumbra;
//    vec3 directionToPosition = normalize(position - vec3(spotLight.pos));
//    vec3 lightDirection = normalize(vec3(spotLight.dir));
//    float x = acos(dot(directionToPosition, lightDirection));

//    if (x <= thetaInner) {
//        return spotLight.color;
//    } else if (thetaInner < x && x <= thetaOuter) {
//        float falloff = -2 * pow((x-thetaInner)/(thetaOuter-thetaInner), 3) + 3 * pow((x-thetaInner)/(thetaOuter-thetaInner), 2);
//        return spotLight.color * (1 - falloff);
//    } else {
//        return vec4(0.0);
//    }
//}

void main() {
    fragColor = texture(texture1, uv);
//    fragColor = vec4(0.0);

//    // ambient term
//    fragColor += ka * ambientColor;

//    for (int i = 0; i < numLights; i++) {
//        float attFactor;
//        vec3 directionToLight;
//        vec4 lightIntensity;

//        Light light = lights[i];
//        if (light.type == 0) { // point
//            float dist = length(vec3(light.pos) - worldPosition);
//            vec3 distVec = vec3(1, dist, pow(dist, 2));
//            // use attenuation
//            attFactor = min(1.0, 1.0 / dot(light.function, distVec));
//            // light has position
//            directionToLight = normalize(vec3(light.pos) - worldPosition);
//            // constant intensity
//            lightIntensity = light.color;
//        } else if (light.type == 1) { // directional
//            // no attenuation
//            attFactor = 1;
//            // light has no position, so just use direction
//            directionToLight = normalize(vec3(-light.dir));
//            // constant intensity
//            lightIntensity = light.color;
//        } else { // spot
//            float dist = length(vec3(light.pos) - worldPosition);
//            vec3 distVec = vec3(1, dist, pow(dist, 2));
//            // use attenuation
//            attFactor = min(1.0, 1.0 / dot(light.function, distVec));
//            // light has position
//            directionToLight = normalize(vec3(light.pos) - worldPosition);
//            // variable intensity according to cone of spot light
//            lightIntensity = spotLightIntensity(light, worldPosition);
//        }

//        vec3 normal = normalize(worldNormal);

//        // diffuse term
//        float cosNormalLight = min(max(dot(normal, directionToLight), 0.0), 1.0);
//        fragColor += attFactor * lightIntensity * kd * diffuseColor * min(max(dot(normal, directionToLight), 0.0), 1.0);

//        // specular term
//        vec3 reflectionDirection = reflect(-directionToLight, normal);
//        vec3 directionToCamera = normalize(vec3(cameraPos) - worldPosition);
//        if (shininess <= 0) {
//            fragColor += attFactor * lightIntensity * ks * specularColor;
//        } else {
//            float cosReflectionCamera = min(max(dot(reflectionDirection, directionToCamera), 0.0), 1.0);
//            fragColor += attFactor * lightIntensity * ks * specularColor * pow(cosReflectionCamera, shininess);
//        }
//    }
}

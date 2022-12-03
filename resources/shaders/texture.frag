#version 330 core

in vec2 uv;

out vec4 fragColor;

uniform sampler2D tex;
uniform int width;
uniform int height;
uniform bool perPixelFilter;
uniform bool kernelBasedFilter;

int kernelSize = 5;

void main() {
    fragColor = texture(tex, uv);

    if (kernelBasedFilter) {
        float redAcc = 0;
        float greenAcc = 0;
        float blueAcc = 0;
        for (int i = -(kernelSize / 2); i <= (kernelSize / 2); i++) {
            for (int j = -(kernelSize / 2); j <= (kernelSize / 2); j++) {
                vec2 adjUV = vec2(min(max(uv[0] + (float(i) / width), 0), 1), min(max(uv[1] + (float(j) / height), 0), 1));
                vec4 adjColor = texture(tex, adjUV);
                redAcc += 1.0 / pow(kernelSize, 2) * adjColor.r;
                greenAcc += 1.0 / pow(kernelSize, 2) * adjColor.g;
                blueAcc += 1.0 / pow(kernelSize, 2) * adjColor.b;
            }
        }
        fragColor.r = redAcc;
        fragColor.g = greenAcc;
        fragColor.b = blueAcc;
    }

    if (perPixelFilter) {
        float gray = 0.299 * fragColor.r + 0.587 * fragColor.g + 0.114 * fragColor.b;
        fragColor.r = gray;
        fragColor.g = gray;
        fragColor.b = gray;
    }
}

#include "Cone.h"

void Cone::updateParams(int param1, int param2) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_param2 = param2;
    setVertexData();
}

glm::vec3 Cone::calcNormal(glm::vec3 v) {
    return glm::normalize(glm::vec3(8 * v.x, -2 * v.y + 1, 8 * v.z));
};

void Cone::makeTile(glm::vec3 topLeft,
                    glm::vec3 topRight,
                    glm::vec3 bottomLeft,
                    glm::vec3 bottomRight,
                    bool cap) {
    glm::vec3 topLeftNorm;
    glm::vec3 bottomLeftNorm;
    glm::vec3 bottomRightNorm;
    glm::vec3 topRightNorm;
    if (cap) {
        topLeftNorm = glm::vec3(0, -1, 0);
        bottomLeftNorm = glm::vec3(0, -1, 0);
        bottomRightNorm = glm::vec3(0, -1, 0);
        topRightNorm = glm::vec3(0, -1, 0);
    } else {
        topLeftNorm = calcNormal(topLeft);
        bottomLeftNorm = calcNormal(bottomLeft);
        bottomRightNorm = calcNormal(bottomRight);
        topRightNorm = calcNormal(topRight);
    }

    insertVec3(m_vertexData, topLeft);
    insertVec3(m_vertexData, topLeftNorm);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);

    insertVec3(m_vertexData, topRight);
    insertVec3(m_vertexData, topRightNorm);
    insertVec3(m_vertexData, bottomLeft);
    insertVec3(m_vertexData, bottomLeftNorm);
    insertVec3(m_vertexData, bottomRight);
    insertVec3(m_vertexData, bottomRightNorm);
}

void Cone::setVertexData() {
    auto cylindrical = [](float r, float theta, float y) {
        return glm::vec3{
            r * sin(theta),
            y,
            r * cos(theta)
        };
    };

    int param1 = std::max(m_param1, 1);
    int param2 = std::max(m_param2, 3);

    float thetaStep = glm::radians(360.f / param2);
    float yStep = 1.0f / param1;
    float rStep = -0.5f / param1;

    for (int i = 0; i < param2; i++) {
        float currentTheta = i * thetaStep;
        float nextTheta = currentTheta + thetaStep;
        // special-case the cone tip
        for (int j = 0; j < param1 - 1; j++) {
            float currentY = j * yStep - 0.5f;
            float nextY = currentY + yStep;
            float currentR = j * rStep + 0.5f;
            float nextR = currentR + rStep;
            // side
            makeTile(cylindrical(nextR, currentTheta, nextY),
                     cylindrical(nextR, nextTheta, nextY),
                     cylindrical(currentR, currentTheta, currentY),
                     cylindrical(currentR, nextTheta, currentY),
                     false);
            // bottom cap
            makeTile(cylindrical(nextR, nextTheta, -0.5f),
                     cylindrical(nextR, currentTheta, -0.5f),
                     cylindrical(currentR, nextTheta, -0.5f),
                     cylindrical(currentR, currentTheta, -0.5f),
                     true);
        }
        // inside of bottom cap
        makeTile(cylindrical(0.0f, nextTheta, -0.5f),
                 cylindrical(0.0f, currentTheta, -0.5f),
                 cylindrical(-rStep, nextTheta, -0.5f),
                 cylindrical(-rStep, currentTheta, -0.5f),
                 true);
        // cone tip
        float tipTheta = currentTheta + (thetaStep / 2);
        glm::vec3 tip = cylindrical(0.0f, tipTheta, 0.5f);
        glm::vec3 bottomLeft = cylindrical(-rStep, currentTheta, 0.5f - yStep);
        glm::vec3 bottomLeftNorm = calcNormal(bottomLeft);
        glm::vec3 bottomRight = cylindrical(-rStep, nextTheta, 0.5f - yStep);
        glm::vec3 bottomRightNorm = calcNormal(bottomRight);
        insertVec3(m_vertexData, tip);
        insertVec3(m_vertexData, glm::normalize(bottomLeftNorm + bottomRightNorm));
        insertVec3(m_vertexData, bottomLeft);
        insertVec3(m_vertexData, bottomLeftNorm);
        insertVec3(m_vertexData, bottomRight);
        insertVec3(m_vertexData, bottomRightNorm);
    }
}

void Cone::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

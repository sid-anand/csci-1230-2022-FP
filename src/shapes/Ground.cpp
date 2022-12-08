#include "Ground.h"
#include "Cube.h"
#include <iostream>

void Ground::updateParams(int param1, int size, int depth) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size;
    m_depth = depth;
    makeGround();
}

void Ground::makeGround() {
    Cube cube = Cube();
    m_vertexData.clear();
    for (int i = -10; i <= m_size + 10; i++) {
        for (int k = 10; k >= -m_depth - 10; k--) {
            cube.updateParams(m_param1, i, -1, k);
            std::vector<float> cube_data = cube.getMesh();
            m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
        }
    }
}

void Ground::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

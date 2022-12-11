#include "Building.h"
#include "Cube.h"
#include "shapes/Cylinder.h"
#include <iostream>

void Building::updateParams(int param1, int size, int height, int depth, float x, float z, int texture) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size;
    m_depth = depth;
    m_height = height;
    m_x = x;
    m_z = z;
    m_texture = texture;
    makeBuilding();
}

void Building::makeBuilding() {
    Cube cube = Cube();
    Cylinder cylinder = Cylinder();

    m_vertexData.clear();

    bool cluster = arc4random() % 2;
    if ((cluster || m_size > 3 || m_depth > 3) && m_size > 2 && m_depth > 2) {
        for (float i = 0.f; i < float(m_size) - 0.5f; i++) {
            for (float k = 0.f; k < float(m_depth) - 0.5f; k++) {
                int towerHeight = arc4random() % m_height + 1;
                float xjitter = float(arc4random() % 50)/100.f;
                float zjitter = float(arc4random() % 50)/100.f;
                i += xjitter;
                k += zjitter;

                bool cylindrical = arc4random() % 2;
                int param2 = arc4random() % 3 + 3;

                for (float j = 0.f; j < float(towerHeight); j++) {
                    if (cylindrical) {
                        cylinder.updateParams(1, param2, i + m_x, j, k + m_z, 0.5f);
                        std::vector<float> cylinder_data = cylinder.getMesh();
                        m_vertexData.insert(m_vertexData.end(), cylinder_data.begin(), cylinder_data.end());
                    } else {
                        cube.updateParams(m_param1, i + m_x, j, k + m_z);
                        std::vector<float> cube_data = cube.getMesh();
                        m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
                    }
                }
            }
        }
    } else {
        float xjitter = float(arc4random() % 50)/100.f;
        float zjitter = float(arc4random() % 50)/100.f;
        m_x += xjitter;
        m_z += zjitter;

        for (float i = 0.f; i < float(m_size); i++) {
            for (float k = 0.f; k < float(m_depth); k++) {
                int towerHeight = arc4random() % m_height + 1;
                for (float j = 0.f; j < float(towerHeight); j++) {
                    cube.updateParams(1, i + m_x, j, k + m_z);
                    std::vector<float> cube_data = cube.getMesh();
                    m_vertexData.insert(m_vertexData.end(), cube_data.begin(), cube_data.end());
                }
            }
        }
    }
}

void Building::insertVec3(std::vector<float> &data, glm::vec3 v) {
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

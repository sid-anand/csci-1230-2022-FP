#include "curvedbuilding.h"
#include "shapes/Cone.h"
#include "shapes/Cylinder.h"

void CurvedBuilding::updateParams(int param1, int size, int height, int depth, float x, float z, int texture) {
    m_vertexData = std::vector<float>();
    m_param1 = param1;
    m_size = size;
    m_height = height;
    m_depth = depth;
    m_x = x;
    m_z = z;
    m_texture = texture;
    makeBuilding();
}

void CurvedBuilding::makeBuilding() {
    Cylinder cylinder = Cylinder();

    m_vertexData.clear();

    for (float j = 0.f; j < 1.f  + (1.f * (m_height - 1)); j++) {
        cylinder.updateParams(m_param1, 25, m_x, j, m_z, m_size);
        std::vector<float> cylinder_data = cylinder.getMesh();
        m_vertexData.insert(m_vertexData.end(), cylinder_data.begin(), cylinder_data.end());
    }

//    Cone cone = Cone();
//    cone.updateParams();
//    std::vector<float> cone_data = cone.getMesh();
//    m_vertexData.insert(m_vertexData.end(), cone_data.begin(), cone_data.end());
}

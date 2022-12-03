#include "sceneparser.h"
#include "scenefilereader.h"
#include "glm/gtx/transform.hpp"

#include <chrono>
#include <memory>
#include <iostream>

bool SceneParser::parse(std::string filepath, RenderData &renderData) {
    ScenefileReader fileReader = ScenefileReader(filepath);
    bool success = fileReader.readXML();
    if (!success) {
        return false;
    }

    renderData.globalData = fileReader.getGlobalData();
    renderData.lights = fileReader.getLights();
    renderData.cameraData = fileReader.getCameraData();

    SceneNode *root = fileReader.getRootNode();
    renderData.shapes.clear();
    glm::mat4 identity(1.0f);
    dfs(root, identity, renderData.shapes);

    return true;
}

void SceneParser::dfs(SceneNode *node, glm::mat4 ctm, std::vector<RenderShapeData> &shapes) {
    glm::mat4 curr = ctm;

    for (SceneTransformation* trans : node->transformations) {
        glm::mat4 mat;
        switch (trans->type) {
            case TransformationType::TRANSFORMATION_TRANSLATE:
                mat = glm::translate(trans->translate);
                break;
            case TransformationType::TRANSFORMATION_SCALE:
                mat = glm::scale(trans->scale);
                break;
            case TransformationType::TRANSFORMATION_ROTATE:
                mat = glm::rotate(trans->angle, trans->rotate);
                break;
            case TransformationType::TRANSFORMATION_MATRIX:
                mat = trans->matrix;
                break;
        }
        curr *= mat;
    }

    for (ScenePrimitive *prim : node->primitives) {
        shapes.push_back(RenderShapeData{ *prim, curr });
    }

    for (SceneNode *child : node->children) {
        dfs(child, curr, shapes);
    }
}

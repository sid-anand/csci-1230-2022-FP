#include "shapes/Building.h"
#include <iostream>
#include <ostream>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class FloorPlan {
    public:
        std::vector<Building> buildings;
        const float streetSize = 1.0f;
        const int maxBlockSize = 4;
        std::vector<int> blockSizesX;
        std::vector<int> blockSizesZ;
        int blockSize = 4;

        FloorPlan(float gridSize, int numTextures, int maxHeight) {
            blockSizesX.clear();
            blockSizesZ.clear();
            // randomly generate x dimension from 2-5
            // randomly generate z dimension from 2-5
            // if there's enough space in that dimension, use it
            float currentSizeX = 0;
            while (currentSizeX < gridSize) {
                int blockSize = (arc4random() % maxBlockSize) + 2;
                while (currentSizeX + blockSize > gridSize) {
                    blockSize -= 1;
                }
                if (blockSize < 2) {
                    break;
                }
                blockSizesX.push_back(blockSize);
                currentSizeX += (blockSize + streetSize);
            }
            float currentSizeZ = 0;
            while (currentSizeZ < gridSize) {
                int blockSize = (arc4random() % maxBlockSize) + 2;
                while (currentSizeZ + blockSize > gridSize) {
                    blockSize -= 1;
                }
                if (blockSize < 2) {
                    break;
                }
                blockSizesZ.push_back(blockSize);
                currentSizeZ += (blockSize + streetSize);

            }
            // vector 1 is x direction, its like [1, 4, 3, 1, 1]
            // vector 2 is z direction, like     [2, 2, 4, 2]
            // xAccumulator and zAccumulator are basically the coordinates of where the block is
            // note it starts at -streetSize rather than 0 because we add streetSize + the buildingSize in the for loop,
            // and we don't have a street for the first block in either dimension
            float xAccumulator = 0;
            for (int i = 0; i < blockSizesX.size(); i++) {
                float buildingDimensionX = blockSizesX[i];
                float zAccumulator = 0;
                for (int j = 0; j < blockSizesZ.size(); j++) {
                    float buildingHeight = arc4random() % maxHeight + 1;
                    Building building = Building();
                    float buildingDimensionZ = blockSizesZ[j];
                    zAccumulator -= (buildingDimensionZ + streetSize);
                    building.updateParams(1, buildingDimensionX, buildingHeight, buildingDimensionZ, xAccumulator, zAccumulator, arc4random() % numTextures);
                    buildings.push_back(building);
                }
                xAccumulator += (buildingDimensionX + streetSize);
            }

        }

};

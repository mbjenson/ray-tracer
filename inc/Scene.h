#pragma once

#include <vector>
#include <stack>

#include "TVec.h"
#include "TVec2f.h"
#include "TRay.h"
#include "TColor.h"
#include "TSphere.h"
#include "TFace.h"
#include "TLight.h"
#include "TMatCol.h"
#include "TTexture.h"

class Scene {
    public:
        // scene attributes
        TVec eye;
        TVec viewDir;
        TVec upDir;
        float vFov;
        int imgWidth, imgHeight;
        TColor bgCol;
        float eta;
        int maxRecDepth;

        std::vector<TMatCol> matCols; // contains material colors
        std::vector<TSphere> sphereVec; // contains spheres
        std::vector<TLight> lightVec; // contains lights
        std::vector<TVec> vertVec; // contains verticies
        std::vector<TVec> normalVec; // contains normal vectors
        std::vector<TVec2f> tVertVec; // contains u v texture coordinates
        std::vector<TFace> faceVec; // contains faces made up of verticies
        std::vector<TTexture> texVec; // contains TTextures
};
#pragma once

#include <iostream>
#include <cmath>

#include "MathFuncs.h"
#include "TVec.h"
#include "TVec2f.h"
#include "TRay.h"
#include "TMatCol.h"
#include "TTexture.h"

/** 
 * Parent class for TFace and TSphere.
*/
class Shape {
    public:

        // *NOTE: see child classes for specific implementations of the virtual functions*

        // calculate surface normal at a point (for spheres)
        virtual TVec getSurfNormal(TVec intPoint){
            return TVec();
        }
        
        // calculates the normal for smooth triangles using the given weights
        virtual TVec getSmoothNormal(TVec hitPoint, float alpha, float beta, float gamma){
            return TVec();
        }

        // bascally gets the u v coordinates for the corresponding texture
        virtual TVec2f getUV(float alpha, float beta, float gamma){
            return TVec2f();
        }

        virtual TVec2f getUV(TVec hitPoint){
            return TVec2f();
        }

        // returns the type of inherited shape type
        virtual int getType(){
            return -1;
        }

        TTexture* texPtr;
        TMatCol* matColPtr;
        int matType; // 0 == material color, 1 == texture
        int matIndex; // index into matCol vector
        int texIndex; // index into texture vector
};
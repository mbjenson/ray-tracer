#pragma once

#include <iostream>
#include "TColor.h"

class TMatCol {
    public:
        // constructor
        // default
        TMatCol();
        // parameterized
        TMatCol(float dr, float dg, float db,
                float sr, float sg, float sb, 
                float ka, float kd, float ks, 
                float n);

        TMatCol(float dr, float dg, float db,
                float sr, float sg, float sb,
                float ka, float kd, float ks,
                float n, float alpha, float eta);
        
        // print to the console
        void print();
        
        // Variables
        TColor color;
        float sr, sg, sb;
        float ka, kd, ks;
        float n; // specular falloff
        float alpha; // translucency
        float eta; // index of refraction
};
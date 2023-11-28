#pragma once

#include <cmath>
#include <iostream>

#include "TVec.h"

class TRay{
    public:
        // constructor
        // default
        TRay();
        // parameterized
        TRay(float x_, float y_, float z_, float dx_, float dy_, float dz_);
        
        TRay(TVec& origin, TVec& dir);

        // normalizes the ray direction
        void normalize();
         // print the ray to the console.
        void print();
        
        // variables
        float x, y, z, dx, dy, dz;
};
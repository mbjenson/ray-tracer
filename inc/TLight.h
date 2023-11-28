#pragma once

#include <iostream>
#include "TColor.h"

class TLight {
    public:
        // constructor
        // default
        TLight();
        // param
        TLight(float x_, float y_, float z_, int w_, float r_, float g_, float b_);

        // print to the console
        void print();

        // variables
        float x, y, z;
        int w; // 1 = point light | 0 = directional light
        TColor color;
};
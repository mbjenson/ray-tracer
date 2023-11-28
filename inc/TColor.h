#pragma once

#include <iostream>

#include <cmath>

class TColor {
    public:
        // constructor
        // def
        TColor();
        // param
        TColor(float r_, float g_, float b_);

        // prints to the console
        void print();

        // operator overloads
        friend TColor operator +(const TColor& u, const TColor& v);
        friend TColor operator -(const TColor& u, const TColor& v);
        friend TColor operator *(const float& c, const TColor& u);
        friend TColor operator /(const TColor& u, const float& c);

        // variables
        float r, g, b;
};
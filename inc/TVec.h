#pragma once

#include <iostream>
#include <cmath>

// 3d vector of floats
class TVec {
    public:
        
        // constructor
        // def
        TVec();
        // param
        TVec(float x_, float y_, float z_);

        // returns the magnitude of the vector
        float getLen();
        // normalize this vector
        void normalize();
        // prints the vector to the console
        void print();
        
        // operator overloads
        friend TVec operator +(const TVec& u, const TVec& v);
        friend TVec operator -(const TVec& u, const TVec& v);
        friend TVec operator *(const float& c, const TVec& u);
        friend TVec operator *(const TVec& u, const float& c);
        friend TVec operator /(const TVec& u, const float& c);

        // variables
        float x, y, z;
};
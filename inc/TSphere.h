#pragma once

#include "Shape.h"

class TSphere: public Shape{
    public:
        /**
         * TSphere default constructor
         * sets all values to 0/nil
        */
        TSphere();

        /**
         * TSphere constructor
         * @param x_, y_, z_ sphere center
         * @param r_ sphere radius
         * @param matColPtr_ pointer to a material color
        */
        TSphere(float x_, float y_, float z_, float r_, TMatCol* matColPtr_);

        /**
         * TSphere constructor
         * @param x_, y_, z_ sphere center
         * @param r_ sphere radius
         * @param matColPtr_ TMatCol pointer
         * @param texPtr_ TTexture pointer
        */
        TSphere(float x_, float y_, float z_, float r_, TMatCol* matColPtr_, TTexture* texPtr_);

        /**
         * get the corresponding UV coordinates from the resulting sphere intersection point
         * @param hitPoint point of intersection with sphere
        */
        TVec2f getUV(TVec hitPoint);

        /**
         * calculates surface normal at a given position on the sphere
         * @param hitPoint point of intersection with the sphere
        */
        TVec getSurfNormal(TVec hitPoint);

        // sphere shape type = 0
        int getType();

        // print sphere's info to the terminal
        void print();

        // vars
        float x, y, z, r;
};
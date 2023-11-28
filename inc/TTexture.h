#pragma once

#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <cmath>

#include "TColor.h"

/**
 * texture class
 * 
 * Contains functionality for reading in image contents from
 * ascii ppm file and storing pixel colors.
 * Colors can be accessed directly via i, j or indirectly via u, v.
*/
class TTexture{
    public:

        /**
         * default constructor that sets everything to 0 or equivalent
        */
        TTexture();

        /**
         * TTexture constructor
         * takes a string 'filename_'.
         * 'filename_' must include the '.ppm' extension
        */
        TTexture(std::string filename_);

        /**
         * reads data from filename into the 'pixels' vector
         * @return 0 for successful loading, 1 for failed loading
        */
        int load();

        /**
         * get pixel color from texture at coords (i, j)
         * @param i, j integer image pixel coordinates for desired pixel
         * @return TColor instance containing color value at (i, j)
        */
        TColor getPix(int i, int j);

        /**
         * get pixel color from loaded texture using u,v texture coordinates
         * @param u, v values >= 0 that are used to calculate the i, j pixel coordinates in the texture.
         * @return TColor instance containing color value at (u, v) coordinates
        */
        TColor getPix(float u, float v);

        /**
         * interpolates the pixel colors surrounding a given UV texture coordinate
         * returns the resultant color
        */
        TColor interp(float u, float v);

        // variables
        bool loaded = false;
        int width, height;
        std::vector<TColor> pixels;
        std::string filename;
        int maxColVal;
};
#include <cmath>
#include "TVec.h"

// returns the normalized version of the input vector   
TVec normalize(TVec u);

// returns the dot product of two vectors
float dot(TVec& u, TVec& v);

// returns the cross product of two vectors
TVec cross(TVec& u, TVec& v);

// clamps a value [x] in range 0 to 1
float clamp(float x);

// returns the magnitude of a vector
float mag(TVec& u);
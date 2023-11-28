#include "MathFuncs.h"

TVec normalize(TVec u){
    float mag = sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2));
    if (mag > 0)
        return u / mag;
    else
        return u;
}

float dot(TVec& u, TVec& v){
    return (u.x * v.x) + (u.y * v.y) + (u.z * v.z);
}

TVec cross(TVec& u, TVec& v){
    return TVec((u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x));
}

float clamp(float x){
    if (x > 1){
        x = 1;
    }
    if (x < 0) {
        x = 0;
    }
    return x;
}

float mag(TVec& u){
    float mag = sqrt(pow(u.x, 2) + pow(u.y, 2) + pow(u.z, 2));
    return mag;
}
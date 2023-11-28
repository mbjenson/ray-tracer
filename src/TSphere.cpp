#include "TSphere.h"

TSphere::TSphere(){
    x = 0.f; y = 0.f; z = 0.f; r = 0.f; matColPtr = NULL;
}

TSphere::TSphere(float x_, float y_, float z_, float r_, TMatCol* matColPtr_){
    x = x_; y = y_; z = z_; r = r_; matColPtr = matColPtr_;
}

TSphere::TSphere(float x_, float y_, float z_, float r_, TMatCol* matColPtr_, TTexture* texPtr_){
    x = x_; y = y_; z = z_; r = r_; matColPtr = matColPtr_; texPtr = texPtr_;
}

TVec2f TSphere::getUV(TVec hitPoint){
    if (matType == 1){
        float phi = acos((hitPoint.z - z)/r);
        float theta = atan2((hitPoint.y - y), (hitPoint.x - x));
        if (theta < 0){
            theta = theta + 2*M_PI;
        }
        float u = theta / (2*M_PI);
        float v = phi / M_PI;
        return TVec2f(v, u);
    }
    return TVec2f();
}

TVec TSphere::getSurfNormal(TVec hitPoint){
    return TVec((hitPoint.x - x)/r, (hitPoint.y - y)/r, (hitPoint.z - z)/r);
}

void TSphere::print(){
    std::cout << "((" << x << ", " << y << ", " << z << "), " << r << ", " << matColPtr << ")\n";
}

int TSphere::getType(){
    return 0;
}
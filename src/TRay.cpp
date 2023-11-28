#include "TRay.h"

TRay::TRay(){
    x = 0.f; y = 0.f; z = 0.f;
    dx = 0.f; dy = 0.f; dz = 0.f;
}

TRay::TRay(float x_, float y_, float z_, float dx_, float dy_, float dz_){
    x = x_; y = y_; z = z_; dx = dx_; dy = dy_; dz = dz_;
}

TRay::TRay(TVec& origin, TVec& dir){
    x = origin.x; y = origin.y; z = origin.z;
    dx = dir.x; dy = dir.y; dz = dir.z;
}

void TRay::normalize(){
    float mag = sqrt(dx * dx + dy * dy + dz * dz);
    dx = dx / mag;
    dy = dy / mag;
    dz = dz / mag;
}

void TRay::print(){
    std::cout << "(" << x << ", " << y << ", " << z << "), (" << dx << ", " << dy << ", " << dz << ")\n";
}
#include "TLight.h"

TLight::TLight(){
    x = 0.f; y = 0.f; z = 0.f;
    w = 1;
    color = TColor();
}

TLight::TLight(float x_, float y_, float z_, int w_, float r_, float g_, float b_){
    x = x_; y = y_; z = z_;
    w = w_;
    color = TColor(r_, g_, b_);
}

void TLight::print(){
    std::cout << "((" << x << ", " << y << ", " << z << "), " << w << ", (" 
                << color.r << ", " << color.g << ", " << color.b << "))\n";
}
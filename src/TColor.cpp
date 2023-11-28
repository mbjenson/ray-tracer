#include "TColor.h"

TColor::TColor(){
    r = 0;
    g = 0;
    b = 0;
}

TColor::TColor(float r_, float g_, float b_){
    r = r_;
    g = g_;
    b = b_;
}

TColor operator +(const TColor& u, const TColor& v){
    TColor result(u.r + v.r, u.g + v.g, u.b + v.b);
    return result;
}

TColor operator -(const TColor& u, const TColor& v){
    TColor result(u.r - v.r, u.g - v.g, u.b - v.b);
    return result;
}

TColor operator *(const float& c, const TColor& u){
    TColor result(u.r * c, u.g * c, u.b * c);
    return result;
}

TColor operator /(const TColor& u, const float& c){
    TColor result(u.r / c, u.g / c, u.b / c);
    return result;
}

void TColor::print(){
    std::cout << "(" << r << ", " << g << ", " << b << ")\n";
}

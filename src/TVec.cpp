#include "TVec.h"

TVec::TVec(){
    x = 0;
    y = 0;
    z = 0;
}

TVec::TVec(float x_, float y_, float z_){
    x = x_;
    y = y_;
    z = z_;
}

float TVec::getLen(){
    return sqrt(x * x + y * y + z * z);
}

void TVec::normalize(){
    float mag = sqrt(x * x + y * y + z * z);
    x = x/mag;
    y = y/mag;
    z = z/mag;
}

void TVec::print(){
    std::cout << "(" << x << ", " << y << ", " << z << ")\n";
}

TVec operator +(const TVec& u, const TVec& v){
    TVec result(u.x + v.x, u.y + v.y, u.z + v.z);
    return result;
}

TVec operator -(const TVec& u, const TVec& v){
    TVec result(u.x - v.x, u.y - v.y, u.z - v.z);
    return result;
}

TVec operator *(const float& c, const TVec& u){
    TVec result(u.x * c, u.y * c, u.z * c);
    return result;
}

TVec operator *(const TVec& u, const float& c){
    TVec result(u.x * c, u.y * c, u.z * c);
    return result;
}

TVec operator /(const TVec& u, const float& c){
    TVec result(u.x / c, u.y / c, u.z / c);
    return result;
}
#include "TMatCol.h"


TMatCol::TMatCol(){
    color = TColor(0.f, 0.f, 0.f);
    sr = 0.f; sg = 0.f; sb = 0.f;
    ka = 0.f; kd = 0.f; ks = 0.f;
    n = 0.f; alpha = 0.f; eta = 0.f;
}

TMatCol::TMatCol(float dr_, float dg_, float db_, float sr_, float sg_, float sb_,
                float ka_, float kd_, float ks_, float n_, float alpha_, float eta_){
    // intrinsic
    color = TColor(dr_, dg_, db_);
    // specular reflective color
    sr = sr_;
    sg = sg_;
    sb = sb_;
    // ambient, diffue, and specular weights
    ka = ka_;
    kd = kd_;
    ks = ks_;
    // specular falloff
    n = n_;
    // alpha value
    alpha = alpha_;
    // index of refraction
    eta = eta_;
}

TMatCol::TMatCol(float dr_, float dg_, float db_,float sr_, float sg_, float sb_, 
                float ka_, float kd_, float ks_, float n_){
    // intrinsic color (r, g, b)
    color = TColor(dr_, dg_, db_);
    // specular reflective color
    sr = sr_;
    sg = sg_;
    sb = sb_;
    // ambient, diffuse, and specular weights
    ka = ka_;
    kd = kd_;
    ks = ks_;
    // specular falloff
    n = n_;
    // alpha value
    alpha = 0.f;
    // index of refraction
    eta = 0.f;
}

void TMatCol::print(){
    std::cout << "((" << color.r << ", " << color.g << ", " << color.b << "), (" 
                << sr << ", "  << sg << ", " << sb << "), (" << ka << ", " << kd << ", " << ks << "), " 
                << n << ", " << alpha << ", " << eta << ")\n";
}


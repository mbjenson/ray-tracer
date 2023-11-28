#include "TFace.h"

TFace::TFace(){
    p0 = TVec(); p1 = TVec(); p2 = TVec();
    vt0 = TVec2f(); vt1 = TVec2f(); vt2 = TVec2f();
    vn0 = TVec(); vn1 = TVec(); vn2 = TVec();

    matType = -1;

    matIndex = -1;
    texIndex = -1;

    matColPtr = 0;
    texPtr = 0;
}

TFace::TFace(TVec p0_, TVec p1_, TVec p2_, int matIndex_){
    // init vectors
    p0 = p0_; p1 = p1_; p2 = p2_; // points
    vt0 = TVec2f(); vt1 = TVec2f(); vt2 = TVec2f(); // u,v texture coords
    vn0 = TVec(); vn1 = TVec(); vn2 = TVec(); // vertex normals

    matType = 0;

    matIndex = matIndex_;
    texIndex = -1;

    matColPtr = 0;
    texPtr = 0;
}

TFace::TFace(TVec p0_, TVec p1_, TVec p2_,
            TVec2f vt0_, TVec2f vt1_, TVec2f vt2_,
            TVec vn0_, TVec vn1_, TVec vn2_, int matIndex_, int texIndex_)
{
    // init vectors
    p0 = p0_; p1 = p1_; p2 = p2_; // points
    vt0 = vt0_; vt1 = vt1_; vt2 = vt2_; // u,v texture coords
    vn0 = vn0_; vn1 = vn1_; vn2 = vn2_; // vertex normals

    matType = 1;

    matIndex = matIndex_;
    texIndex = texIndex_;

    matColPtr = 0;
    texPtr = 0;
    // info bools
    usingVN = true;
    usingVT = true;
}

TFace::TFace(TVec p0_, TVec p1_, TVec p2_, TVec2f vt0_, TVec2f vt1_, TVec2f vt2_, int matIndex_, int texIndex_){
    // init vectors
    p0 = p0_; p1 = p1_; p2 = p2_; // points
    vt0 = vt0_; vt1 = vt1_; vt2 = vt2_; // u,v texture coords
    vn0 = TVec(); vn1 = TVec(); vn2 = TVec(); // vertex normals
    
    matType = 1;

    matIndex = matIndex_;
    texIndex = texIndex_;

    matColPtr = 0;
    texPtr = 0;
    // info bools
    usingVT = true;
}

TFace::TFace(TVec p0_, TVec p1_, TVec p2_, TVec vn0_, TVec vn1_, TVec vn2_, int matIndex_){
    // init vectors
    p0 = p0_; p1 = p1_; p2 = p2_; // points
    vt0 = TVec2f(); vt1 = TVec2f(); vt2 = TVec2f(); // u,v texture coords
    vn0 = vn0_; vn1 = vn1_; vn2 = vn2_; // vertex normals
    
    matType = 0;

    matIndex = matIndex_;
    texIndex = -1;

    texPtr = 0;
    matColPtr = 0;
    // info bools
    usingVN = true;
}

TVec TFace::getSurfNormal(TVec hitPoint){
    TVec e1(p1 - p0);
    TVec e2(p2 - p0);
    return cross(e1, e2);
}

TVec TFace::getSmoothNormal(TVec hitPoint, float alpha, float beta, float gamma){
    if (usingVN){ // check if vertex normals are provided
        TVec N(alpha*vn0 + beta*vn1 + gamma*vn2);
        N.normalize();
        return N;
    }
    // if not using Vertex normals then get the regular surface normal
    return getSurfNormal(hitPoint);
}

TVec2f TFace::getUV(float alpha, float beta, float gamma){
    if (usingVT){
        float u = alpha*vt0.u + beta*vt1.u + gamma*vt2.u;
        float v = alpha*vt0.v + beta*vt1.v + gamma*vt2.v;
        return TVec2f(u, v);
    }
    return TVec2f();
}

int TFace::getType(){
    return 1;
}
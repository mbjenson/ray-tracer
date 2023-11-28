#pragma once

#include "Shape.h"

class TFace: public Shape{
    public:
        /**
         * TFace Constructor
         * sets all values to default/nil
        */
        TFace();

        /**
         * TFace constructor
         * @param p0_, p1_, p1_ are the three vertecies of the triangle.
         * @param matIndex_ index into material colors vector
        */
        TFace(TVec p0_, TVec p1_, TVec p2_, int matIndex_);

        /**
         * TFace Constructor
         * @param p0_, p1_, p2_ TVec triangle verticies
         * @param vt0_, vt1_, vt2_ TVec2f texture coordinates for triangle verticies
         * @param vn0_, vn1_, vn2_ TVec normal directions for each of triangles vertecies
         * @param matIndex_ index into material colors vector
         * @param texIndex_ index into texture vector
        */
        TFace(  TVec p0_, TVec p1_, TVec p2_,
                TVec2f vt0_, TVec2f vt1_, TVec2f vt2_,
                TVec vn0, TVec vn1, TVec vn2, int matIndex_, int texIndex_);

        /**
         * Constructor for TFace
         * @param p0_, p1_, p2_ TVec specfiying triangle verticies
         * @param vt0_, vt1_, vt2_ TVec2f specifying texture coordinates (u, v)
         * @param matIndex_ index into material colors vector
         * @param texIndex_ index into texture vector
        */
        TFace(TVec p0_, TVec p1_, TVec p2_, TVec2f vt0_, TVec2f vt1_, TVec2f vt2_, int matIndex_, int texIndex_);

        /**
         * Constructor for TFace
         * @param p0_, p1_, p2_ TVec specfiying triangle verticies
         * @param vn0_, vn1_, vn2_ TVec normal directions for each of triangles vertecies
         * @param matIndex_ index into material colors vector
        */
        TFace(TVec p0_, TVec p1_, TVec p2_, TVec vn0, TVec vn1, TVec vn2, int matIndex_);

        // METHODS

        /**
         * calculate surface normal direction for a given point
         * @param hitPoint : TVec shape intersection point
        */
        TVec getSurfNormal(TVec hitPoint);

        // calculates the surface normal using the barycentric weights and vert normals for each vertex
        /**
         * if the face uses vertex normals, a surface normal is calculated based on those values
         * @param hitPoint : TVec shape intersection
         * @param alpha, beta, gamma : barycentric coordinates
        */
        TVec getSmoothNormal(TVec hitPoint, float alpha, float beta, float gamma);

        /**
         * calculates the uv coordinates given the weights
         * @param alpha, beta, gamma: barycentric weights
        */
        TVec2f getUV(float alpha, float beta, float gamma);

        // face shape type = 1
        int getType();
        
        // boolean flags for determining what information was provided for a face
        bool usingVN = false; // if face has vertex normals 
        bool usingVT = false; // if face has texture coords

        // vars
        TVec p0, p1, p2, vn0, vn1, vn2;
        TVec2f vt0, vt1, vt2;
};
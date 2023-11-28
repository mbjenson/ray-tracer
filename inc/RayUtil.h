
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <string>
#include <vector>
#include <stdlib.h>

#include "TRay.h"
#include "TVec.h"
#include "TMatCol.h"
#include "TTexture.h"
#include "TColor.h"
#include "TFace.h"
#include "TLight.h"
#include "Shape.h"
#include "TSphere.h"
#include "TVec2f.h"
#include "MathFuncs.h"
#include "Scene.h"
#include "constants.h"


/**
 * Traces a given ray into the scene.
 * Checks for intersection with faces and spheres and returns and returns the appropriately lit color.
 * @param ray the ray to best cast
 * @param depth number of remaining ray reflections
 * @param shape a shape to check against to avoid self intersection
 * @param S scene object with all scene information
 * 
 * @return a TColor struct with the pixels color
*/

TColor traceRay(TRay ray, Scene& scene, Shape* thisShape, int depth);


/**
 * Determines the color of a pixel based on a hit object.
 * 
 * @param shape shape class instance which is the parent to all other types of shapes.
 * @param hitPoint where ray cast intersected the object.
 * @param ray originally cast ray.
 * @param weights barycentric weights (alpha, beta, gamma) calculated for the hitpoint on the triangle.
 *                weights are only used for triangles.
 * @param S scene object containing all scene information
 * @param depth int remaining depth of recussion
 * 
 * @return TColor stuct of a pixels color
*/
TColor shadeRay(Shape* shape, TVec& hitPoint, TRay& ray, TVec& weights, Scene& scene, int depth);


/**
 * traces a ray to a light and returns amount of light that reaches that point from the light (0-1)
 * @param ray a TRay from the shape intersection point and in direction of the 'TLight light'
 * @param light checking if this light makes it to the surface of the shape.
 * @param self pointer to self; Objects cannot cast shadows on themselves.
 * @param S scene object containing all scene
*/
float traceShadow(TRay ray, TLight light, Shape* shape, Scene& S);

#include "RayUtil.h"

using namespace constants;

TColor traceRay(TRay ray, Scene& S, Shape* thisShape, int depth){

    if (depth <= 0){ // if past max recursion depth
        return TColor(0, 0, 0);
    }

    Shape* solution = NULL;
    float leastT = 1000000; // past the possible viewing range
    TVec baryWeights; // barycentric coordinates for possible ray-triangle intersection point

    // check for intersection with spheres
    for (int i = 0; i < S.sphereVec.size(); i++){
        if (thisShape == &(S.sphereVec[i])) // avoid self intersection
          continue;
        TSphere cur = S.sphereVec[i];
        float A = 1.f; // A = 1 (optimization possible b/c dir is always unit length)
        float B = 2*(ray.dx*(ray.x - cur.x) + ray.dy*(ray.y - cur.y) + ray.dz*(ray.z - cur.z));
        float C = pow(ray.x - cur.x, 2) + pow(ray.y - cur.y, 2) + pow(ray.z - cur.z, 2) - pow(cur.r, 2);
        // calculate discriminant
        float disc = pow(B, 2) - (4*A*C);
        // calculate ray-sphere intersection points
        if (disc > 0 && disc < EPS){ // one intersection point
            float t = (-1*B)/(2*A);
            if (t < leastT && t > EPS){
                leastT = t;
                solution = &S.sphereVec[i];
            }
            else {
                continue;
            }
        }
        if (disc > 0){ // two intersection points
            float t1 = ((-1*B) + sqrt(disc))/(2*A);
            float t2 = ((-1*B) - sqrt(disc))/(2*A);
            if (t1 > EPS && t1 <= leastT){
                    leastT = t1;
                    solution = &S.sphereVec[i];
            }
            if (t2 > EPS && t2 <= leastT){
                    leastT = t2;
                    solution = &S.sphereVec[i];
            }
            else {
                continue;
            }
        }
        else { // no intersection with this sphere
            continue;
        }
    }
    // check for intersection with triangles (faces)
    for(int i = 0; i < S.faceVec.size(); i++){
        if (thisShape == &(S.faceVec[i])) // avoid self intersection
           continue;
        // first: evaluate ray-plane intersection with the plane formed by the triangle's edges
        TVec p0 = S.faceVec[i].p0;
        TVec p1 = S.faceVec[i].p1;
        TVec p2 = S.faceVec[i].p2;
        // triangle edges
        TVec e1 = p1 - p0;
        TVec e2 = p2 - p0;
        // plane surface normal
        TVec n = cross(e1, e2);

        float D = -1*(n.x * p0.x) - (n.y * p0.y) - (n.z * p0.z);
        float denom = n.x * ray.dx + n.y * ray.dy + n.z * ray.dz;
        if (denom == 0)
            continue;
        
        float numer = -1*(n.x * ray.x + n.y * ray.y + n.z * ray.z + D);
        float t = numer / denom;
        if (t <= 0)
            continue;
        
        TVec planeInt(ray.x + t*ray.dx, ray.y + t*ray.dy, ray.z + t*ray.dz);
        // calculating the barycentric coordinates
        TVec ep = planeInt - p0;
        // creating 2x2 system to solve for Beta and Gamma
        float d11 = dot(e1, e1);
        float d12 = dot(e1, e2);
        float d22 = dot(e2, e2);
        float d1p = dot(e1, ep);
        float d2p = dot(e2, ep);
        // determinant of the 2x2 system
        float det = d11 * d22 - d12 * d12;
        if (det == 0) // no solution and thus no intersection
            continue;
        // barycentric weights
        float beta = (d22 * d1p - d12 * d2p)/det;
        float gamma = (d11 * d2p - d12 * d1p)/det;
        float alpha = 1 - gamma - beta;
        // triangle area
        float area = 0.5*(mag(n));
        // proportions of area
        float littleA = area * alpha;
        float littleB = area * beta;
        float littleC = area * gamma;
        if (littleA < 0 || littleB < 0 || littleC < 0)
            continue;
        // verify point is within triangle
        else {
            if (t < leastT && t > EPS){
                leastT = t;
                solution = &S.faceVec[i];
                baryWeights = TVec(alpha, beta, gamma);
            }
        }
    }
    // intersection point
    TVec interPt(ray.x + ray.dx * leastT, ray.y + ray.dy * leastT, ray.z + ray.dz * leastT);
    // call shade ray to get this pixel's color
    return shadeRay(solution, interPt, ray, baryWeights, S, depth);
}

TColor shadeRay(Shape* shape, TVec& hitPoint, TRay& ray, TVec& weights, Scene& S, int depth){
    // if no intersection, return the background color
    if (shape == NULL){ 
        return S.bgCol;
    }

    TColor thisFinalCol;
    TVec N = shape->getSurfNormal(hitPoint); // calculate surface normal [N], at given point
    if (shape->getType() == 1){ // if shape is a triangle
        // if triangle uses vertex specific normals, calc the normal based on those
        N = shape->getSmoothNormal(hitPoint, weights.x, weights.y, weights.z);
    }
    N.normalize();

    // V = unit vector that points towards ray origin
    TVec V = normalize(TVec(ray.x - hitPoint.x, ray.y - hitPoint.y, ray.z - hitPoint.z)); 

    TMatCol curCol = *(shape->matColPtr); // local copy of color
    TColor diffuseCol;
    if (shape->matType == 1){ // if shape uses a texture Calculate UV coordinates
        // for spheres
        TVec2f thisUV;
        if (shape->getType() == 0){
            thisUV = shape->getUV(hitPoint);
        }
        // for triangles
        else if (shape->getType() == 1){
            thisUV = shape->getUV(weights.x, weights.y, weights.z);
        }
        // set the diffuse color accordingly from texture color
        diffuseCol = shape->texPtr->getPix(thisUV.u, thisUV.v);
        diffuseCol = diffuseCol/255.f; // scale color down to range [0:1]
    }
    else if (shape->matType == 0){ // if no texture used, use material color's diffuse color
        diffuseCol = curCol.color;
    }
    float r = 0.f;
    float g = 0.f;
    float b = 0.f;
    // loop through the lightVec and compute the color value for this pixel based on the lights
    for (int i = 0; i < S.lightVec.size(); i++){
        float SH = 1.f; // shadow value

        TVec lightDir(S.lightVec[i].x, S.lightVec[i].y, S.lightVec[i].z);
        // compute L vector (away from surface int point towards the light)
        TVec L;
        if (S.lightVec[i].w == 0){ // directional light
            L = normalize(-1 * lightDir);
        }
        else { // point light
            L = normalize(lightDir - hitPoint);
        }
        TVec H = normalize(L + V); // compute H vector (halway vector between light and viewer)
        TRay toTheLight(hitPoint.x, hitPoint.y, hitPoint.z, L.x, L.y, L.z); // ray from int-point to the light
        SH = traceShadow(toTheLight, S.lightVec[i], shape, S);
        // add value for each light
        r += clamp(SH * S.lightVec[i].color.r * clamp(curCol.kd * diffuseCol.r * clamp(dot(N, L)) + curCol.ks * curCol.sr * clamp(pow(dot(N, H), curCol.n))));
        g += clamp(SH * S.lightVec[i].color.g * clamp(curCol.kd * diffuseCol.g * clamp(dot(N, L)) + curCol.ks * curCol.sg * clamp(pow(dot(N, H), curCol.n))));
        b += clamp(SH * S.lightVec[i].color.b * clamp(curCol.kd * diffuseCol.b * clamp(dot(N, L)) + curCol.ks * curCol.sb * clamp(pow(dot(N, H), curCol.n))));
    }
    TVec I = normalize(TVec(-1*ray.dx, -1*ray.dy, -1*ray.dz)); // ray pointing towards origin of incident ray
    float cosThetaI = dot(N, I);
    float etaI, etaT;
    if (cosThetaI < 0) { // if exiting object
        N = -1*N;
        cosThetaI = dot(N, I);
        // set value of current medium
        etaI = curCol.eta; 
        etaT = S.eta;
    }
    else { // if entering object
        etaI = S.eta;
        etaT = curCol.eta;
    }

    TVec Rdir = (2*dot(N, I)*N) - I; // compute reflected ray direction
    TRay R(hitPoint, Rdir);

    TColor reflectedCol; // globally illuminated reflection value
    if (curCol.ks > 0){  // check for fully non reflective surface
        reflectedCol = traceRay(R, S, shape, depth-1);
    }
    else {
        reflectedCol = TColor(0, 0, 0);
    }

    float F0 = pow((curCol.eta - 1)/(curCol.eta + 1), 2);
    float Fr = F0 + (1 - F0)*pow((1 - dot(I, N)), 5);
    float sinThetaI = sqrt(1 - pow(cosThetaI, 2));
    float etaRat = etaI/etaT;

    TColor refractedCol;
    if ((etaT/etaI) - sinThetaI > EPS){ // check if not in TIR (total internal reflectance)
        TVec Tdir = normalize((-1*(N)*sqrt(1 - (pow(etaRat, 2)*sinThetaI))) + (etaRat)*((cosThetaI*N) - I));
        //TVec Tdir = (-1*(N)*sqrt(1 - (pow(etaRat, 2)*sinThetaI))) + (etaRat)*((cosThetaI*N) - I);
        TRay T(hitPoint.x, hitPoint.y, hitPoint.z, Tdir.x, Tdir.y, Tdir.z);
        refractedCol = traceRay(T, S, shape, depth-1);
    }
    else {
        refractedCol = reflectedCol;
    }
    // calculate final color values
    thisFinalCol.r = clamp((curCol.ka * diffuseCol.r + r) + (Fr * reflectedCol.r) + (1 - Fr)*(1 - curCol.alpha)*refractedCol.r);
    thisFinalCol.g = clamp((curCol.ka * diffuseCol.g + g) + (Fr * reflectedCol.g) + (1 - Fr)*(1 - curCol.alpha)*refractedCol.g);
    thisFinalCol.b = clamp((curCol.ka * diffuseCol.b + b) + (Fr * reflectedCol.b) + (1 - Fr)*(1 - curCol.alpha)*refractedCol.b);
    return thisFinalCol;
}

float traceShadow(TRay ray, TLight light, Shape* self, Scene& S){
    float SH = 1.f; // 1 = not in shadow, 0 = in shadow
    // distance to the light (only used with point lights)
    float lightDist = sqrt(pow(light.x - ray.x, 2) + pow(light.y - ray.y, 2) + pow(light.z - ray.z, 2));
    // spheres
    for (int i = 0; i < S.sphereVec.size(); i++){
        if (&(S.sphereVec[i]) == self){
            continue;
        }
        TSphere curSphere = S.sphereVec[i];
        TMatCol curCol = *(curSphere.matColPtr); // local copy of a shape's material properties
        float A = 1; // A = 1 (optimization b/c ray dir is always normalized)
        float B = 2*(ray.dx*(ray.x - curSphere.x) + ray.dy*(ray.y - curSphere.y) + ray.dz*(ray.z - curSphere.z));
        float C = pow(ray.x - curSphere.x, 2) + pow(ray.y - curSphere.y, 2) + pow(ray.z - curSphere.z, 2) - pow(curSphere.r, 2);
        float disc = pow(B, 2) - (4*A*C); // calculate discriminant
        // check for both types of light
        if (light.w == 1){ // if it is a point light
            // find the light's distance away from current ray-surface intersection point
            if (disc == 0){ // one solution
                // if t is > 0 and < light Dist then we are being blocked from this light.
                float t = (-1*B)/(2*A);
                if (t > EPS && t < lightDist){
                    SH = SH * (1 - curCol.alpha);
                }
            }
            if (disc > 0){ // two solutions
                float t1 = ((-1*B) + sqrt(disc))/(2*A);
                float t2 = ((-1*B) - sqrt(disc))/(2*A);
                if (t1 > EPS && t1 < lightDist){
                    SH = SH * (1 - curCol.alpha);
                }
                else if (t2 > EPS && t2 < lightDist){
                    SH = SH * (1 - curCol.alpha);
                }
            }
            if (disc < 0){ // no solution
                continue;
            }
        }
        if (light.w == 0){ // if it is a directional light
            if (disc == 0){ // one solution
                float t = (-1*B)/(2*A);
                if (t > EPS){
                    SH = SH * (1 - curCol.alpha);
                }
                else {
                    continue;
                }
            }
            if (disc > 0){ // two solutions
                // if either t1 or t2 is positive, we are blocked from this light.
                float t1 = ((-1*B) + sqrt(disc))/(2*A);
                float t2 = ((-1*B) - sqrt(disc))/(2*A);
                if (t1 > EPS || t2 > EPS){
                //if (t1 > 0 || t2 > 0){
                    SH = SH * (1 - curCol.alpha);
                }
                else {
                    continue;
                }
            }
            if (disc < 0){ // no solution
                continue;
            }
        }
    }
    // triangles
    for (int i = 0; i < S.faceVec.size(); i++){
        if (&(S.faceVec[i]) == self){ // avoid self intersection
            continue;
        }
        TFace curFace = S.faceVec[i];
        TMatCol curCol = *(curFace.matColPtr);
        // verts
        TVec p0 = curFace.p0;
        TVec p1 = curFace.p1;
        TVec p2 = curFace.p2;
        // face edges
        TVec e1 = p1 - p0;
        TVec e2 = p2 - p0;
        // surface normal dir
        TVec n = cross(e1, e2);

        float D = -1*(n.x * p0.x) - (n.y * p0.y) - (n.z * p0.z);

        float denom = n.x * ray.dx + n.y * ray.dy + n.z * ray.dz;
        if (denom == 0)
            continue;
        float numer = -1*(n.x * ray.x + n.y * ray.y + n.z * ray.z + D);
        float t = numer / denom;

        if (t <= EPS)
            continue;

        TVec planeInt(ray.x + t*ray.dx, ray.y + t*ray.dy, ray.z + t*ray.dz);

        // calculating the barycentric coordinates
        TVec ep = planeInt - p0;
        // calculating for system of equations note: d12 == d21
        float d11 = dot(e1, e1);
        float d12 = dot(e1, e2);
        float d22 = dot(e2, e2);
        float d1p = dot(e1, ep);
        float d2p = dot(e2, ep);
        // determinant of the 2x2 system
        float det = d11 * d22 - d12 * d12;
        if (det == 0){ // no solution and thus no intersection
            continue;
        }
        
        float beta = (d22 * d1p - d12 * d2p)/det;
        float gamma = (d11 * d2p - d12 * d1p)/det;
        float alpha = 1 - gamma - beta;

        float area = 0.5*(mag(n));

        float littleA = area * alpha;
        float littleB = area * beta;
        float littleC = area * gamma;
        
        if (littleA < 0 || littleB < 0 || littleC < 0){
            continue;
        }
        else {
            if (light.w == 0){ // directional light
                if (t > EPS){
                    SH = SH * (1 - curCol.alpha);
                }
                continue;
            }
            if (light.w == 1){ // point lights
                if (t > EPS && t < lightDist){
                    SH = SH * (1 - curCol.alpha);
                }
                continue;
            }
        }
    }
    return SH;
}
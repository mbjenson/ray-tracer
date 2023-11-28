
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <cmath>
#include <vector>
#include <sstream>

#include "TVec.h"
#include "TVec2f.h"
#include "TColor.h"
#include "TMatCol.h"
#include "TLight.h"
#include "TRay.h"
#include "TSphere.h"
#include "TFace.h"
#include "TTexture.h"
#include "MathFuncs.h"
#include "Shape.h"
#include "RayUtil.h"
#include "Scene.h"
#include "inputUtil.h"

#define PI 3.14159265

/**
 * Main function for the ray tracer
 * Takes a [filename.txt] containing a scene description and renders the scene to an ascii ppm file
*/
int main(int argc, char* argv[]) {
    // phase 1: to recieve input or not to receive input? that is the question...

    // check for file
    if (argv[1] == NULL){
        std::cout << "No Input File Provided\n";
        return 0;
    }
    // opening the file
    std::ifstream inputF(argv[1]);
    // check for invalid file name
    if (!inputF.good()){
        std::cout << "Invalid File Name: " << argv[1] << "\n";
        return 0;
    }
    // Create output file name
    std::string fileName = argv[1];
    std::string::size_type pos = fileName.find('.');
    if (pos != std::string::npos){
        fileName = fileName.substr(0, pos);
    }
    fileName.append(".ppm");

    // Scene information
    const int recDepth = 10;
    Scene S;
    S.maxRecDepth = recDepth;

    // File Information Bools
    int hasEye = 0;
    int hasViewDir = 0;
    int hasUpDir = 0;
    int hasVFov = 0;
    int hasImgWidth = 0;
    int hasImgHeight = 0;
    int hasImSize = 0;
    int hasBGCol = 0;
    int hasCurCol = 0;
    int hasCurTex = 0;

    // reading information from the text file (meticulous check for valid input)
    if (inputF.is_open())
    {
        std::string line;
        while(getline(inputF, line)){
            std::istringstream ss(line); // setting the line as a string stream object so is it parsable
            std::string word;
            while(ss >> word) {
                if (word.find('#') != std::string::npos){
                    while (ss >> word){
                        // skipping through commment..
                    }
                }
                else if (word == "eye"){
                    hasEye = 1; // note that 'eye' is present in the input file
                    for (int a = 0; a < 3; a++){ // get three values for vector
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // check if it is a valid number
                                float tempVal = stof(word);
                                if (a == 0)
                                    S.eye.x = tempVal; // convert to float
                                if (a == 1)
                                    S.eye.y = tempVal;
                                if (a == 2)
                                    S.eye.z = tempVal;
                            }
                            else {
                                std::cout << "invalid value for 'eye'\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for 'eye'\n";
                            return -1;
                        }
                    }
                }
                else if (word == "viewdir"){
                    hasViewDir = 1; // note that viewdir is present in the file
                    for (int a = 0; a < 3; a++){ // get three pieces of information
                        if (ss >> word){ // get next word in the line
                            if (isFloat(word)){ // check if it is a valid number
                                float tempVal = stof(word);
                                if (a == 0)
                                    S.viewDir.x = tempVal; // convert to float
                                if (a == 1)
                                    S.viewDir.y = tempVal;
                                if (a == 2)
                                    S.viewDir.z = tempVal;
                            }
                            else {
                                std::cout << "invalid value for 'viewdir'\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for 'viewdir'\n";
                            return -1;
                        }
                    }
                }
                else if (word == "updir"){
                    hasUpDir = 1; // note that the updir is present in the input file
                    for (int a = 0; a < 3; a++){ // get three pieces of information
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // check for valid number
                                float tempVal = stof(word);
                                if (a == 0)
                                    S.upDir.x = tempVal; // conver to float
                                if (a == 1)
                                    S.upDir.y = tempVal;
                                if (a == 2)
                                    S.upDir.z = tempVal;
                            }
                            else {
                                std::cout << "invalid value for 'updir'\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for 'updir'\n";
                            return -1;
                        }
                    }
                }
                else if (word == "vfov"){
                    hasVFov = 1; // note that vfov is present in the input file
                    if (ss >> word){ // get the next word in the line
                        if (isFloat(word)){ // check for valid number
                            S.vFov = stof(word); // convert to float
                            if (S.vFov < 0){
                                std::cout << "vfov must be greater that 0\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for 'vfov'. Please ensure vFov is a degree amount\n";
                            return -1;
                        }
                    }
                    else {
                        std::cout << "invalid value for 'vfov'. Please ensure vFov is a degree amount\n";
                        return -1;
                    }
                }
                else if (word == "imsize"){
                    hasImSize = 1;
                    for (int a = 0; a < 2; a++){ // get two numbers for imsize
                        if (ss >> word){ // get next item in the line
                            if (isInt(word)){ // check if valid whole number
                                int tempVal = stoi(word); // convert to int
                                if (tempVal < 0){ // check for invalid number
                                    std::cout << "image dimensions must be greater than 0\n";
                                    return -1;
                                }
                                if (a == 0){
                                    S.imgWidth = tempVal;
                                    hasImgWidth = 1;
                                }
                                if (a == 1){
                                    S.imgHeight = tempVal;
                                    hasImgHeight = 1;
                                }
                            }
                            else {
                                std::cout << "invalid image dimensions\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid image dimensions\n";
                            return -1;
                        }
                    }
                }
                else if (word == "bkgcolor"){
                    hasBGCol = 1; // note that bkgCol is present in the input file
                    for (int a = 0; a < 4; a++){ // get three numbers (a simple color)
                        if (ss >> word){ // get next word in the line
                            if (isFloat(word)){ // check for valid number
                                float tempVal = stof(word); // convert to float
                                if (a != 3){
                                    if (tempVal > 1.0 || tempVal < 0){ // check for invalid number
                                        std::cout << "color values must be in range 0 to 1\n";
                                        return -1;
                                    }
                                    if (a == 0)
                                        S.bgCol.r = tempVal;
                                    if (a == 1)
                                        S.bgCol.g = tempVal;
                                    if (a == 2)
                                        S.bgCol.b = tempVal;
                                }
                                else {
                                    if (tempVal < 0){
                                        std::cout << "background eta cannot be less than 0\n";
                                        return -1;
                                    }
                                    if (a == 3)
                                        S.eta = tempVal;
                                }
                            }
                            else {
                                std::cout << "invalid background color, \nmust be floating point number in range 0 to 1\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid background color, \nmust be floating point number in range 0 to 1\n";
                            return -1;
                        }
                    }
                }
                else if (word == "mtlcolor"){
                    hasCurCol = 1; // note that at least one material color is present in the file at this point
                    TMatCol newMatCol;
                    for (int a = 0; a < 12; a++){ // get the three pieces of information for the color
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // ensure a floating point value
                                float tempVal = stof(word);
                                if (a < 9){ // the value for n can be larger than 1
                                    if (tempVal > 1.0 || tempVal < 0) { // check for valid color values
                                        std::cout << "color values must be in range 0 to 1\n";
                                        return -1;
                                    }
                                }
                                if (a == 0)
                                    newMatCol.color.r = tempVal;
                                if (a == 1)
                                    newMatCol.color.g = tempVal;
                                if (a == 2)
                                    newMatCol.color.b = tempVal;
                                if (a == 3)
                                    newMatCol.sr = tempVal;
                                if (a == 4)
                                    newMatCol.sg = tempVal;
                                if (a == 5)
                                    newMatCol.sb = tempVal;
                                if (a == 6)
                                    newMatCol.ka = tempVal;
                                if (a == 7)
                                    newMatCol.kd = tempVal;
                                if (a == 8)
                                    newMatCol.ks = tempVal;
                                if (a == 9)
                                    newMatCol.n = tempVal;
                                if (a == 10)
                                    newMatCol.alpha = tempVal;
                                if (a == 11)
                                    newMatCol.eta = tempVal;
                            }
                            else {
                                std::cout << "invalid rgb material color value(s), \nall values besides n, alpha, and eta must be floating point numbers in range 0 to 1\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid rgb material color value(s), \nall values besides n, alpha, and eta must be floating point number in range 0 to 1\n";
                            return -1;
                        }
                    }
                    // add color to the vector of material colors
                    S.matCols.push_back(newMatCol);
                }
                else if (word == "texture"){
                    TTexture newTex;
                    if (ss >> word){
                        newTex.filename = word;
                    }
                    else {
                        std::cout << "No filename provided for texture\n";
                        return -1;
                    }
                    // loading texture
                    if (newTex.load()){ // if texture loading fails
                        std::cout << "failed to load texture\n";
                        return -1;
                    }
                    S.texVec.push_back(newTex);
                    hasCurTex = 1;
                }
                else if (word == "light"){
                    TLight thisLight; // create new light struct
                    for (int a = 0; a < 7; a++){
                        if (ss >> word){
                            if (isFloat(word)){
                                float tempVal = stof(word);
                                if (a == 0)
                                    thisLight.x = tempVal;
                                if (a == 1)
                                    thisLight.y = tempVal;
                                if (a == 2)
                                    thisLight.z = tempVal;
                                if (a == 3){ // for w flag
                                    if (isInt(word)){
                                        int wVal = stoi(word);
                                        if (wVal != 1 && wVal != 0){
                                            std::cout << "invalid w value.\nMust be 0 or 1\n";
                                            return -1;
                                        }
                                        thisLight.w = wVal;
                                    }
                                }
                                if (a > 3){
                                    if (tempVal > 1.0 || tempVal < 0){
                                        std::cout << "invalid input for light color\nmust be in range 0 to 1.0\n";
                                        return -1;
                                    }
                                    if (a == 4)
                                        thisLight.color.r = tempVal;
                                    if (a == 5)
                                        thisLight.color.g = tempVal;
                                    if (a == 6)
                                        thisLight.color.b = tempVal;
                                }
                            }
                            else {
                                std::cout << "invalid light value(s)\nnote that rgb values must be in range 0 to 1.\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid light value(s)\nnote that rgb values must be in range 0 to 1.\n";
                            return -1;
                        }
                    }
                    S.lightVec.push_back(thisLight); // add light to vector of lights
                }
                else if (word == "sphere"){
                    TSphere thisSphere; // create empty sphere struct
                    for (int a = 0; a < 4; a++){ // getting 4 pieces of information
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // check if float value
                                float tempVal = stof(word); // convert to float
                                if (a == 0)
                                    thisSphere.x = tempVal;
                                if (a == 1)
                                    thisSphere.y = tempVal;
                                if (a == 2)
                                    thisSphere.z = tempVal;
                                if (a == 3)
                                    thisSphere.r = tempVal;
                            }
                            else {
                                std::cout << "invalid sphere dimensions\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid sphere dimensions\n";
                            return -1;
                        }
                    }
                    // check for corresponding color and texture or just color
                    if (hasCurTex && hasCurCol){
                        thisSphere.matType = 1;
                        thisSphere.matIndex = S.matCols.size()-1;
                        thisSphere.texIndex = S.texVec.size()-1;
                    }
                    else if (hasCurCol){
                        thisSphere.matType = 0;
                        thisSphere.matIndex = S.matCols.size()-1;
                    }
                    else {
                        std::cout << "ensure shapes are listed under a material color or texture\n";
                        return -1;
                    }
                    S.sphereVec.push_back(thisSphere);
                }
                else if (word == "v"){
                    TVec thisVert;
                    for (int a = 0; a < 3; a++){
                        if (ss >> word){
                            if (isFloat(word)){
                                float tempVal = stof(word);
                                if (a == 0)
                                    thisVert.x = tempVal;
                                if (a == 1)
                                    thisVert.y = tempVal;
                                if (a == 2)
                                    thisVert.z = tempVal;
                            }
                            else {
                                std::cout << "invalid vertex coordinates\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid vertex coordinates\n";
                            return -1;
                        }
                    }
                    S.vertVec.push_back(thisVert);
                }
                else if (word == "vn"){
                    TVec thisN;
                    for (int a = 0; a < 3; a++){ // get three values for vector
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // check if it is a valid number
                                float tempVal = stof(word);
                                if (a == 0)
                                    thisN.x = tempVal; // convert to float
                                if (a == 1)
                                    thisN.y = tempVal;
                                if (a == 2)
                                    thisN.z = tempVal;
                            }
                            else {
                                std::cout << "invalid value for 'normal vector'\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for 'normal vector'\n";
                            return -1;
                        }
                    }
                    S.normalVec.push_back(thisN);
                }
                else if (word == "vt"){
                    TVec2f thisVec2f;
                    for (int a = 0; a < 2; a++){ // get three values for vector
                        if (ss >> word){ // get the next word in the line
                            if (isFloat(word)){ // check if it is a valid number
                                float tempVal = stof(word);
                                if (a == 0)
                                    thisVec2f.u = tempVal;
                                if (a == 1)
                                    thisVec2f.v = tempVal;
                            }
                            else {
                                std::cout << "invalid value for texture u v coords\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid value for texture u v coords\n";
                            return -1;
                        }
                    }
                    S.tVertVec.push_back(thisVec2f);
                }
                else if (word == "f"){
                    // data arrays
                    int v[3];
                    int vt[3];
                    int vn[3];
                    // flags to determine what information should be provided for face verticies
                    int bVT = 0;
                    int bVN = 0;
                    // local copies of the sizes of vertVec, normalVec, and tVertVec
                    int vertCount = S.vertVec.size();
                    int normalCount = S.normalVec.size();
                    int tVertCount = S.tVertVec.size();
                    // loop through each vertex declaration
                    for (int a = 0; a < 3; a++){
                        if (ss >> word){
                            const char* cWord = word.c_str();
                            if (sscanf(cWord, "%d/%d/%d", &v[a], &vt[a], &vn[a]) == 3){
                                // handling bad input
                                if (v[a] > vertCount || vt[a] > tVertCount || vn[a] > normalCount ||
                                    v[a] < 1 || vt[a] < 1 || vn[a] < 1)
                                {
                                    std::cout << "error in face values\n";
                                    return -1;
                                }
                                // set flags
                                if (a == 0){
                                    bVT = 1;
                                    bVN = 1;
                                }
                                // handling bad input
                                else {
                                    if (bVT != 1 || bVN != 1){
                                        std::cout << "inconsistent formatting in face declaration\n";
                                        return -1;
                                    }
                                }
                            }
                            else if (sscanf(cWord, "%d//%d", &v[a], &vn[a]) == 2){
                                // handling bad input
                                if (v[a] > vertCount || vn[a] > normalCount ||
                                    v[a] < 1 || vn[a] < 1)
                                {
                                    std::cout << "error in face values\n";
                                    return -1;
                                }
                                // set flags
                                if (a == 0){
                                    bVT = 0;
                                    bVN = 1;
                                }
                                // handling bad input
                                else {
                                    if (bVT != 0 || bVN != 1){
                                        std::cout << "inconsistent formatting in face declaration\n";
                                        return -1;
                                    }
                                }
                            }
                            else if (sscanf(cWord, "%d/%d", &v[a], &vt[a]) == 2){
                                // handling bad input
                                if (v[a] > vertCount || vt[a] > tVertCount ||
                                    v[a] < 1 || vt[a] < 1)
                                {
                                    std::cout << "error in face values\n";
                                    return -1;
                                }
                                // set flags
                                if (a == 0){
                                    bVT = 1;
                                    bVN = 0;
                                }
                                // handling bad input
                                else {
                                    if (bVT != 1 || bVN != 0){
                                        std::cout << "inconsistent formatting in face declaration\n";
                                        return -1;
                                    }
                                }
                            }
                            else if (sscanf(cWord, "%d", &v[a]) == 1){
                                // handling bad input
                                if (v[a] > vertCount || v[a] < 1)
                                {
                                    std::cout << "error in face values\n";
                                    return -1;
                                }
                                if (a == 0){
                                    // set flags
                                    bVT = 0;
                                    bVN = 0;
                                }
                                // handling bad input
                                else {
                                    if (bVT != 0 || bVN != 0){
                                        std::cout << "inconsistent formatting in face declaration\n";
                                        return -1;
                                    }
                                }
                            }
                            else {
                                std::cout << "invalid face formatting\n";
                                return -1;
                            }
                        }
                        else {
                            std::cout << "invalid Face declaration.\n";
                            return -1;
                        }
                    }

                    // make TFace object from values provided
                    TFace thisFace;
                    if (hasCurTex && hasCurCol){ // if has texture must have color and corresponding texture coordinates
                        if (bVT && bVN){
                            thisFace = TFace(
                                S.vertVec[v[0] - 1], S.vertVec[v[1] - 1], S.vertVec[v[2] - 1],
                                S.tVertVec[vt[0] - 1], S.tVertVec[vt[1] - 1], S.tVertVec[vt[2] - 1],
                                S.normalVec[vn[0] - 1], S.normalVec[vn[1] - 1], S.normalVec[vn[2] - 1],
                                S.matCols.size()-1, S.texVec.size()-1);
                        }
                        else if (bVT && !bVN){
                            thisFace = TFace(
                                S.vertVec[v[0] - 1], S.vertVec[v[1] - 1], S.vertVec[v[2] - 1],
                                S.tVertVec[vt[0] - 1], S.tVertVec[vt[1] - 1], S.tVertVec[vt[2] - 1],
                                S.matCols.size()-1, S.texVec.size()-1);
                        }
                    }
                    if (hasCurCol){ // if no corresponding texture exists or does not have vertex texture coordinates
                        if (!bVT && bVN){
                            thisFace = TFace(
                                S.vertVec[v[0] - 1], S.vertVec[v[1] - 1], S.vertVec[v[2] - 1],
                                S.normalVec[vn[0] - 1], S.normalVec[vn[1] - 1], S.normalVec[vn[2] - 1],       
                                S.matCols.size()-1);
                        }
                        else if (!bVT && !bVN){
                            thisFace = TFace(
                                S.vertVec[v[0] - 1], S.vertVec[v[1] - 1], S.vertVec[v[2] - 1],       
                                S.matCols.size()-1);
                        }
                    }
                    else {
                        std::cout << "face has no corresponding texture or material color\n";
                        return -1;
                    }
                    S.faceVec.push_back(thisFace);
                }
            }
        }
        inputF.close(); // close the input file

        // using material index to get pointer to corresponding material
        // Spheres
        for (int i = 0; i < S.sphereVec.size(); i++){
            if (S.sphereVec[i].matType == 0) { // material color
                S.sphereVec[i].matColPtr = &S.matCols[S.sphereVec[i].matIndex];
            }
            if (S.sphereVec[i].matType == 1) {// texture
                S.sphereVec[i].matColPtr = &S.matCols[S.sphereVec[i].matIndex];
                S.sphereVec[i].texPtr = &S.texVec[S.sphereVec[i].texIndex];
            }
        }
        // Faces
        for (int i = 0; i < S.faceVec.size(); i++){
            if (S.faceVec[i].matType == 0) {// material color
                S.faceVec[i].matColPtr = &S.matCols[S.faceVec[i].matIndex];
            }
            if (S.faceVec[i].matType == 1) {// texture
                S.faceVec[i].matColPtr = &S.matCols[S.faceVec[i].matIndex];
                S.faceVec[i].texPtr = &S.texVec[S.faceVec[i].texIndex];
            }
        }
        // ensuring each essential item was included in the input file.
        if (!hasEye) {
            std::cout << "missing eye vector in input file\n";
            return -1;
        }
        if (!hasViewDir){
            std::cout << "missing view direction vector in input file\n";
            return -1;
        }
        if (!hasUpDir){
            std::cout << "missing up direction vector in input file\n";
            return -1;
        }
        if (!hasVFov){
            std::cout << "missing horizontal field of view in input file\n";
            return -1;
        }
        if (!hasImSize){
            std::cout << "missing imsize in input file\n";
            return -1;
        }
        if (!hasImgWidth){
            std::cout << "missing image width in input file\n";
            return -1;
        }
        if (!hasImgHeight){
            std::cout << "missing image height in input file\n";
            return -1;
        }
        if (!hasBGCol){
            std::cout << "missing background color in input file\n";
            return -1;
        }
    }
    else {
        std::cout << "Unable to open file\n";
    }
    // creating new file with same name as input
    std::ofstream file;
    file.open(fileName);
    if (!file){
        std::cout<<"Error in creating file!";
        return -1;
    }
    // PPM header
    file << "P3\n";
    file << "# Drawn by Matthew Jenson's Ray Tracer\n";
    file << S.imgWidth << " ";
    file << S.imgHeight << "\n";
    file << "255\n";
    
    // Phase 2: of lines and their traces
    S.upDir.normalize(); // normalize the up dir just in case
    S.viewDir.normalize(); // normalize the view dir just in case
    // checking to upDir and  viewDir are colinear or opposite (may change later)
    if (S.viewDir.x == S.upDir.x && S.viewDir.y == S.upDir.y && S.viewDir.z == S.upDir.z){
        std::cout << "the viewDir and upDir are colinear, please avoid this\n";
        return -1;
    }
    if (S.viewDir.x == -1*S.upDir.x && S.viewDir.y == -1*S.upDir.y && S.viewDir.z == -1*S.upDir.z){
        std::cout << "the viewDir and UpDir are opposite lines, please avoid this\n";
        return -1;
    }
    TVec u = normalize(cross(S.viewDir, S.upDir)); // calculate u vector
    TVec v = normalize(cross(u, S.viewDir)); // calculate v vector 

    float camDist = 5; // Setting viewing distance (I am using 5)
    // Getting aspect ratio
    float aspectRat = (float)S.imgWidth / (float)S.imgHeight; // convert to float to ensure floating point division

    // Getting viewing window dimensions in world coordinates
    float h = 2*camDist*(tan((S.vFov/2) * (PI/180.0))); // height
    float w = h * aspectRat; // width

    // Calculating window corners in world coordinates
    TVec winWorldCenter(S.eye.x + (camDist * S.viewDir.x), S.eye.y + (camDist * S.viewDir.y), S.eye.z + (camDist * S.viewDir.z));
    TVec vHalfH(v.x * (h/2), v.y * (h/2), v.z * (h/2)); // vector v times half of height
    TVec uHalfW(u.x * (w/2), u.y * (w/2), u.z * (w/2)); // vector u time half of width
    // upper left corner of window
    TVec UL ( winWorldCenter.x + vHalfH.x - uHalfW.x, 
                winWorldCenter.y + vHalfH.y - uHalfW.y,
                winWorldCenter.z + vHalfH.z - uHalfW.z);

    // upper right corner of winder
    TVec UR ( winWorldCenter.x + vHalfH.x + uHalfW.x,
                winWorldCenter.y + vHalfH.y + uHalfW.y,
                winWorldCenter.z + vHalfH.z + uHalfW.z);
    
    // lower left corner of window
    TVec LL ( winWorldCenter.x - vHalfH.x - uHalfW.x,
                winWorldCenter.y - vHalfH.y - uHalfW.y,
                winWorldCenter.z - vHalfH.z - uHalfW.z);
    
    // define step between pixels on horizonal and vertical
    TVec dh((UR.x - UL.x)/(S.imgWidth-1), (UR.y - UL.y)/(S.imgWidth-1), (UR.z - UL.z)/(S.imgWidth-1));
    TVec dv((LL.x - UL.x)/(S.imgHeight-1), (LL.y - UL.y)/(S.imgHeight-1), (LL.z - UL.z)/(S.imgHeight-1));

    // calculating each pixel color
    for (int j = 0; j < S.imgHeight; j++){ 
        for (int i = 0; i < S.imgWidth; i++){

            // current pixel position
            TVec curPixelPos(UL.x + (i * dh.x) + (j * dv.x), UL.y + (i * dh.y) + (j * dv.y), UL.z + (i * dh.z) + (j * dv.z));

            // calculate normal dir from the origin to the current pixel in the viewing window
            TRay rayDir(S.eye.x, S.eye.y, S.eye.z, curPixelPos.x - S.eye.x, curPixelPos.y - S.eye.y, curPixelPos.z - S.eye.z);
            rayDir.normalize();

            // call traceRay on the new rayDirection vector & provide the sphereVector and background color
            TColor rayColor = traceRay(rayDir, S, NULL, recDepth);

            // scaling the color to be an int between 0 and 255
            TColor finalPixCol((int)(rayColor.r * 255), (int)(rayColor.g * 255), (int)(rayColor.b * 255));

            // write value to file
            file << finalPixCol.r << " " << finalPixCol.g << " " << finalPixCol.b;
            // every 4 pixels, we will write to a new line to make the ppm readable
            if (i % 4)
                file << " ";
            else
                file << "\n";
        }
    }
    file.close();
    return 0;
}

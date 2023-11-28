# ray-tracer

Ray tracer scene renderer created entirely with C++

## Contents
1. [**Sample Scene**](#sample-scene)
2. [**Capabilities**](#capabilities)
3. [**Build Instructions**](#build)
4. [**Scene Description File**](#scene-description-file)
5. [**Program Structure**](#program-structure)

## Sample scene
![](sample_scene/t4.jpg)

## Capabilities
* Render a scene from a [scene description file](#scene-description-file)
* spheres and triangles
* transparent and opaque objects
* reflective objects
* textured objects

## Build
*Note: uses g++ compiler and c++11*

#### Clone the repository

    git clone https://github.com/mbjenson/ray-tracer.git
#### compile
compiled binary executable located in the project's home directory
    
    make
#### Run
Run executable with a [scene description file](#scene-description-file) to render the scene to an ascii ppm file

    ./raytracer path/to/scenedescriptionfile.txt
## Scene Description File
*see [sample scene](https://github.com/mbjenson/ray-tracer/blob/main/sample_scene/t4.txt) for a description file example. The output is displayed [above](#sample-scene) or can be viewed [here](https://github.com/mbjenson/ray-tracer/blob/main/sample_scene/t4.jpg)*
### Settup
Every scene file needs to include the following information to settup the scene:

**image size**: two ints defining the image width and height in pixels

    'imsize [width] [height]'
**eye position**: 3D float vector

    'eye [x] [y] [z]'
**viewing direction vector**: 3D float vector

    'viewdir [x] [y] [z]'
**up direction**: 3D float vector

    'updir [x] [y] [z]'
**vertical field of view**: positive float

    'vfov [x]'
**background scene color**: the color drawn as the background

    'bkgcol [r] [g] [b] [η]'
where the η (eta) allows the user to specify a density-surround that the camera is emerssed in. (eg. water)

### Shapes, Materials, and Lights

**Materials**

        'mtlcol [r] [g] [b] [specular_r] [specular_g] [specular_b] [ambient_weight] [diffuse_weight] [specular_weight] [specular_falloff] [alpha] [index_of_refraction]'      
* r, g, b : intrisic object color (0 to 1)
* specular_r, specular_g, specular_b : color of specular reflection (0 to 1)
* ambient_weight : intensity of color without light (0 to 1)
* diffuse_weight : intensity of color when in light (0 to 1)
* specular_weight : intensity of specular reflection (0 to 1; if > 0, object will show reflections)
* specular_falloff : controls the weight of how large the area of specular reflection is (>= 0)
* alpha : controls the transparency of the material (0 to 1)
* index_of_refraction : how much a ray is bent upon entry into the object (> 0)

**Spheres**

    'sphere [x_center] [y_center] [z_center] [radius]'
all values are floats

**Faces (triangles)**

Faces have several options and components

verticies : declared in the file and, upon execution, are stored in an array.

    'v [x_cord] [y_cord] [z_cord]'

faces : combination of 3 verticies which have been previously declared in the file. vertices are references in order of declaration in the file starting at 1.

    'f [v1] [v2] [v3]'

WIP left off at defining using vertex normals and texture coordinates in face defnitiions
    

## Program Structure

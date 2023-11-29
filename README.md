# ray-tracer

Ray tracer scene renderer created entirely with C++

## Contents

1. [**Legend**](#legend)
2. [**Sample Scene**](#sample-scene)
3. [**Capabilities**](#capabilities)
4. [**Build Instructions**](#build)
5. [**Scene Description File**](#scene-description-file)
6. [**Program Structure**](#program-structure)

## Legend

* '[]' brackets specify a user defined value. for example 'sphere [x] [y] [z] [r]' could be written as 'sphere 2.5 1 5.0 1' in practice.


## Sample scene
![](sample_scene/t4.jpg)

## Capabilities
* Render a scene from a [scene description file](#scene-description-file)
* spheres and triangles
* transparent and opaque objects
* reflective objects
* textured objects

## Build
*Note: uses g++ for compilation and written in c++11*

#### Clone the repository

    git clone https://github.com/mbjenson/ray-tracer.git
#### compile
compiled binary executable located in the project's home directory
    
    make
#### Run
Running the executable with a [scene description file](#scene-description-file) will create an ascii ppm (.ppm) containing the rendered image under the same basename as the input file. The output file is located in the same directory as the input file. If a ppm file with that name already exists there, it will be overridden.
to run the executable with an input file, use the following command

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
**background scene color**: 4D positive floats

    'bkgcol [r] [g] [b] [η]'
where the η (eta) allows the user to specify a density-surround that the camera is emerssed in. (eg. water)

### Shapes, Materials, Lights, and Textures

**Textures**
The textures used in the ray tracer must be in ascii ppm format and are declared as follows

    'texture path/to/texture.ppm file'

**Materials**
objects use the last defined material they are listed under in the file.

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

verticies : declared in the file and, upon execution, are stored in an array. These verticies are referenced later when defining faces. 3D float vector

    'v [x] [y] [z]'
vertex normals : a vertex can have a corresponding normal direction associated with it. this allows for smoothly shaded triangles. 3D float vector

    'vn [x] [y] [z]'
texture coordinates : a vertex can have a corresponding texture coordinate assocated with it which corresponds to where a texture is positioned in reference to that vertex. values (0 - 1)

    'vt [u] [v]'

faces : combination of 3 verticies which have been previously declared in the file. vertices are references in order of declaration in the file starting at 1.

    
    'f [v1] [v2] [v3]'

faces with vertex normals : include a vertex normal for every vertex

    'f [v1]//[vn1] [v2]//[vn2] [v3]//[vn3]
faces with texture coordinates : include a texture coordinate for each vertex

    'f [v1]/[vt1] [v2]/[vt2] [v3]/[vt3]
faces with both texture coordinates and vertex normals

    'f [v1]/[vt1]/[vn1] [v2]/[vt2]/[vn2] [v3]/[vt3]/[vn3]
## Program Structure
The header files contain general documentation and information about specific implementation can be found in the cpp files.

## Mathematics
Lighting is calculated with an extended version of the blinn-phong illumination equation.

# Final Programming Project

## Table of Contents

1. Build
2. Run
3. Shaders
   3.1. Depth Shader
   3.2. Lighting Shader
   3.3. Shadow Test Shader
   3.4. Texture Shader
   3.5. Wood Shader
4. Classes
   4.1. Camera
   4.2. Plane
   4.3. Shader
   4.4. Texture
   4.5. Volume
5. Attribution
   5.1. Brickwork_001
   5.2. Hardwood (Diffuse)
   5.3. Hardwood (Normal)
   5.4. Window
6. Project Structure

## Build

`$ make`

## Run

`$ ./bin/main`

## Shaders

### Depth Shader

The depth shader is used to render the shadow map. It simply transforms coordinates to light-space and
renders the z-coordinate of the fragment to a texture.

### Lighting Shader

A generic lighting shader that functions as a base implementation for materials and lights.

### Shadow Test Shader

A shader that renders a shadow map to a surface for debugging purposes.

### Texture Shader

This is by-far the biggest shader, implementing directional and point lighting, and materials sampled
from textures. It supports diffuse, specular, normal, and shadow maps.

### Wood Shader

The wood shader is only used for the table in the scene and generates a calculated wood texture using
noise functions.

**Noise functions:**

float random(vec2): Standard seeded random function, returning the fractional component of a sin
function multiplied by arbitrary numbers. The dot product condenses the 2D input down to a float.

float noise(vec2): Noise functions.

mat2 rotate2d(float): Creates a 2D rotation matrix for an arbitrary angle

float lines(vec2, float): Add lines to the output based on the input coordinates

The main() function calculates the pattern for a coordinate ahead of time, passing the result to the
lighting functions which use it as the material color in light calculations.

## Classes

### Camera

The camera class is just an object-oriented approach to managing the matrices used in the shaders.

### Plane

The plane class is used to create a quad comprised of two triangles - mostly for convenience.

### Shader

The shader class was only extended to support Geometry shaders. I added this while attempting to
implement cubemap shadows, but didn't end up using them. The second constructor with Geometry
shader was left in place as it may be useful in future.

### Texture

Same texture class as previous assignments, minimal (if any?) modifications made.

### Volume

The volume class combines 6 planes to make a rectangular prism.

## Attribution

Textures used in this project are licenced under CC-BY, see below for individual details. Note that I created
my own specular maps for the Hardwood and Window textures using GIMP.

### Brickwork_001

Files: Brickwork_001_Diffuse.png, Brickwork_001_Normal.png, Brickwork_001_Specular.png

Source: [Open Game Art](https://opengameart.org/content/brickwork001-seamless)

Licence: [CC-BY 4.0](https://creativecommons.org/licenses/by/4.0/)

Created By: [PagDev](https://pagdev.de.cool/) - [Open Game Art Profile](http://opengameart.org/users/pagdev)

No modifications made.

### Hardwood (Diffuse)

Files: Hardwood_Diffuse.png

Source: [Open Game Art](https://opengameart.org/content/tiling-hardwood-floor-texture-1024x1024)

Licence: [CC-BY 3.0](https://creativecommons.org/licenses/by/3.0/)

Created By: [bart](https://opengameart.org/users/bart)

No modifications made.

### Hardwood (Normal)

Files: Hardwood_Normal.png

Source: [Open Game Art](https://opengameart.org/content/tiling-hardwood-floor-texture-1024x1024-hardwoodjpg-normal-map)

Licence: [CC-BY 3.0](https://creativecommons.org/licenses/by/3.0/)

Created By: [wulax](https://opengameart.org/users/wulax)

No modifications made.

### Window

Files: Window_Diffuse.png, Window_Normal.png

Source: [Open Game Art](https://opengameart.org/content/repeating-mini-windows-largish-seamless-texture-with-normalmap)

Licence: [CC-BY 3.0](https://creativecommons.org/licenses/by/3.0/)

Created By: [Keith333](https://opengameart.org/users/keith333)

No modifications made.

## Project Structure

./
|- bin
| '- main
|- include
| '- ...
|- lib
| |- libGL.so
| |- libGLEW.so.2.0.0
| |- libglfw.so.3.2
| '- stb_image.h
|- screenshots
| |- screenshot1.png
| |- screenshot2.png
| '- screenshot3.png
|- shaders
| |- depth.frag
| |- depth.vert
| |- lighting.frag
| |- lighting.vert
| |- shadow_test.frag
| |- shadow_test.vert
| |- texture.frag
| |- texture.vert
| |- wood.frag
| '- wood.vert
|- textures
| |- Brickwork_001_Diffuse.png
| |- Brickwork_001_Normal.png
| |- Brickwork_001_Specular.png
| |- Hardwood_Diffuse.png
| |- Hardwood_Normal.png
| |- Hardwood_Specular.png
| |- Window_Diffuse.png
| |- Window_Normal.png
| '- Window_Specular.png
|- camera.cpp
|- camera.h
|- main.cpp
|- makefile
|- plane.cpp
|- plane.h
|- README.md
|- shader.cpp
|- shader.h
|- texture.cpp
|- texture.h
|- volume.cpp
'- volume.h

# Assignment 5 Notes

## Shader class

The Shader class has been updated to link the shader program in the constructor rather than
in the `Program()` method as before. It also has the capability to access uniform variables
by name from a map, adding the id to said map if it is not already present. Aside from these
modifications, the Shader class is the same as previous assignments.

## Lighting

Light direction is calculated in world space using the values from the normal map directly.
The red tint is applied by multiplying each component of the output by a varying factor.

## Project Structure

./
|- bin
|- images
| |- normal.png
| '- skyline.png
|- include
| |- ...
|- lib
| |- libGL.so
| |- libGLEW.so.2.0.0
| |- libglfw.so.3.2
| |- stb_image.h
|- shaders
| |- reflection.frag
| |- reflection.vert
| |- texture.frag
| |- texture.vert
|- main.cpp
|- makefile
|- README.md
|- shader.cpp
|- shader.h
|- texture.cpp
|- texture.h

## Build

$ make

## Run

$ ./bin/main

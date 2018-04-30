# Final Programming Project

## Shaders

### Brick Uniforms

**Vertex:**

mat4 Model
mat4 View
mat4 Projection

**Fragment:**

Light lights[]:

* vec3 position
* vec3 ambient
* vec3 diffuse
* vec3 specular
* float constant
* float linear
* float quadratic

DirLight dirLight:

* vec3 direction
* vec3 ambient
* vec3 diffuse
* vec3 specular

vec3 CameraPosition

Material material:

* sampler2D diffuse
* sampler2D specular
* float shininess

sampler2D shadowMap

## Attribution

Textures used in this project are licenced under CC-BY, see below for individual details.

### Brickwork_001

Source: [Open Game Art](https://opengameart.org/content/brickwork001-seamless)

Licence: [CC-BY](https://creativecommons.org/licenses/by/4.0/)

Created By: [PagDev](https://pagdev.de.cool/) - [Open Game Art Profile](http://opengameart.org/users/pagdev)

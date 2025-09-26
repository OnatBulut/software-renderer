#ifndef __COMMON_H__
#define __COMMON_H__

#include <cglm/cglm.h>

#include "vector.h"


typedef struct
{
    vec4 p;     // x,y,z,w
    vec3 n;     // i,j,k
    vec4 c;     // r,g,b,a
    vec3 t;     // u,v,w
} Vertex;

typedef struct
{
    Vertex v[3];
    bool smooth;
} Triangle;

CREATE_VECTOR_TYPE(Triangle, triangle)
typedef struct
{
    vector_triangle tris;
} Mesh;


#endif //__COMMON_H__

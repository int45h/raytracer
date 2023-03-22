#pragma once
#include <cmath>

float max(float a, float b){return (a>b)?a:b;}
float min(float a, float b){return (a<b)?a:b;}
float clamp(float x, float l, float u){return min(max(x, l), u);}

float lerp(float a, float b, float t){return a+(b-a)*t;}

#define Sc_max(a,b)     max(a,b)
#define Sc_min(a,b)     min(a,b)
#define Sc_clamp(x,l,u) clamp(x,l,u)
#define Sc_lerp(a,b,t)  lerp(a,b,t)

typedef union vec4
{
    float xyzw[4];
    struct{float x,y,z,w;};
}
vec4;
#define new_vec4(x,y,z,w) (vec4){x,y,z,w};

vec4 vadd(float *a, float *b)
{
    return (vec4)
    {
        .x=a[0] + b[0],
        .y=a[1] + b[1],
        .z=a[2] + b[2],
        .w=a[3] + b[3]
    };
}

vec4 vsub(float *a, float *b)
{
    return (vec4)
    {
        .x=a[0] - b[0],
        .y=a[1] - b[1],
        .z=a[2] - b[2],
        .w=a[3] - b[3]
    };
}

vec4 vmul(float *a, float *b)
{
    return (vec4)
    {
        .x=a[0] * b[0],
        .y=a[1] * b[1],
        .z=a[2] * b[2],
        .w=a[3] * b[3]
    };
}

vec4 vdiv(float *a, float *b)
{
    return (vec4)
    {
        .x=a[0] / b[0],
        .y=a[1] / b[1],
        .z=a[2] / b[2],
        .w=a[3] / b[3]
    };
}

float vlength(float *v)
{
    return sqrt
    (
        v[0]*v[0] + 
        v[1]*v[1] + 
        v[2]*v[2] + 
        v[3]*v[3]
    );
}

float vlength_sq(float *v)
{
    return 
    (
        v[0]*v[0] + 
        v[1]*v[1] + 
        v[2]*v[2] + 
        v[3]*v[3]
    );
}

float vinvlen(float *v)
{
    return 1 / sqrt
    (
        v[0]*v[0] + 
        v[1]*v[1] + 
        v[2]*v[2] + 
        v[3]*v[3]
    );
}

vec4 vmax(float *a, float *b)
{
    return (vec4)
    {
        .x=Sc_max(a[0], b[0]),
        .y=Sc_max(a[1], b[1]),
        .z=Sc_max(a[2], b[2]),
        .w=Sc_max(a[3], b[3])
    };
}

vec4 vmin(float *a, float *b)
{
    return (vec4)
    {
        .x=Sc_max(a[0], b[0]),
        .y=Sc_max(a[1], b[1]),
        .z=Sc_max(a[2], b[2]),
        .w=Sc_max(a[3], b[3])
    };
}

vec4 vclamp(float *x, float *l, float *u)
{
    return (vec4)
    {
        .x=Sc_clamp(x[0], l[0], u[0]),
        .y=Sc_clamp(x[1], l[1], u[1]),
        .z=Sc_clamp(x[2], l[2], u[2]),
        .w=Sc_clamp(x[3], l[3], u[3])
    };
}

vec4 vlerp(float *a, float *b, float t)
{
    return (vec4)
    {
        .x=Sc_lerp(a[0], b[0], t),
        .y=Sc_lerp(a[1], b[1], t),
        .z=Sc_lerp(a[2], b[2], t),
        .w=Sc_lerp(a[3], b[3], t)
    };
}

#pragma once
#include <cmath>
#include <cstdint>

#include "common_math.h"
#include "common_gfx.h"

#define NEW_VECTOR(x,y,z,w)     (vec4){(float)x,(float)y,(float)z,(float)w}
#define VECTOR_ZERO             (vec4){0,0,0,0}
#define VECTOR_ONE              (vec4){1,1,1,0}
#define NEW_EULER_RAD(y,p,r)    (vec4){_180_PI*(float)x,_180_PI*(float)y,_180_PI*(float)z,0}
#define NEW_EULER_DEG(y,p,r)    (vec4){(float)x,(float)y,(float)z,0}
#define Vw 3
#define Vx 0
#define Vy 1
#define Vz 2

#define VECTOR_BASIC_OP(va,vb,op) (vec4){\
va[0] op vb[0],\
va[1] op vb[1],\
va[2] op vb[2],\
va[3] op vb[3]}
#define VECTOR_SCALAR_OP(va, b, op) (vec4){\
va[0] op b,\
va[1] op b,\
va[2] op b,\
va[3] op b}
#define VECTOR_DOT(a,b) (\
a[0]*b[0] + \
a[1]*b[1] + \
a[2]*b[2] + \
a[3]*b[3])

#define VECTOR_FUNCTION_OP1(a,op) (vec4){\
(float)op(a[0]),\
(float)op(a[1]),\
(float)op(a[2]),\
(float)op(a[3])}
#define VECTOR_FUNCTION_OP2(a,b,op) (vec4){\
(float)op(a[0], b[0]),\
(float)op(a[1], b[1]),\
(float)op(a[2], b[2]),\
(float)op(a[3], b[3])}
#define VECTOR_FUNCTION_OP3(a,b,c,op) (vec4){\
(float)op(a[0], b[0], c[0]),\
(float)op(a[1], b[1], c[1]),\
(float)op(a[2], b[2], c[2]),\
(float)op(a[3], b[3], c[3])}

#define RGBA8888_TO_VECTOR(RGBA) NEW_VECTOR(    \
    CHANNEL(RGBA,R)/255.9f,                     \
    CHANNEL(RGBA,G)/255.9f,                     \
    CHANNEL(RGBA,B)/255.9f,                     \
    CHANNEL(RGBA,A)/255.9f)
#define VECTOR_TO_RGBA8888(v) (                 \
        (((int)(v[0]*255.9f))<<24)|             \
        (((int)(v[1]*255.9f))<<16)|             \
        (((int)(v[2]*255.9f))<<8)|              \
        (((int)(v[3]*255.9f))))

typedef union vec4
{
    float xyzw[4];
    struct{float x,y,z,w;};
}
vec4;
#define new_vec4(x,y,z,w) (vec4){x,y,z,w};

vec4 vadd(float *a, float *b)   {return VECTOR_BASIC_OP(a, b, +);}
vec4 vsub(float *a, float *b)   {return VECTOR_BASIC_OP(a, b, -);}
vec4 vmul(float *a, float *b)   {return VECTOR_BASIC_OP(a, b, *);}
vec4 vdiv(float *a, float *b)   {return VECTOR_BASIC_OP(a, b, /);}
vec4 vcross(float *a, float *b)
{
    return NEW_VECTOR
    (
        +a[1]*b[2]-a[2]*b[1], 
        -a[0]*b[2]-a[2]*b[0], 
        +a[0]*b[1]-a[1]*b[0], 
        0
    );
}
float vdot(float *a, float *b)  {return VECTOR_DOT(a,b);}
float vlength(float *v)         {return sqrt(VECTOR_DOT(v, v));}
float vlength_sq(float *v)      {return VECTOR_DOT(v, v);}
float vinvlen(float *v)         {return 1 / sqrt(VECTOR_DOT(v, v));}
vec4 vnorm(float *a)
{
    float inv_len = vinvlen(a);
    return VECTOR_SCALAR_OP(a, inv_len, *);
}
vec4 vmax(float *a, float *b)               {return VECTOR_FUNCTION_OP2(a, b, Sc_max);}
vec4 vmin(float *a, float *b)               {return VECTOR_FUNCTION_OP2(a, b, Sc_min);}
vec4 vclamp(float *x, float *l, float *u)   {return VECTOR_FUNCTION_OP3(x, l, u, Sc_clamp);}
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
vec4 vsin(float *v){return VECTOR_FUNCTION_OP1(v, sin);}
vec4 vcos(float *v){return VECTOR_FUNCTION_OP1(v, cos);}
vec4 vtan(float *v){return VECTOR_FUNCTION_OP1(v, tan);}

vec4 RGBA_to_vec4(uint32_t RGBA)    {return RGBA8888_TO_VECTOR(RGBA);}
uint32_t vec4_to_RGBA(float *v)     {return VECTOR_TO_RGBA8888(v);}

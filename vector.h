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

#define NEW_VECTOR(x,y,z,w) (vec4){(float)x,(float)y,(float)z,(float)w}
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

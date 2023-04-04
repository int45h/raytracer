#pragma once
#include "vector.h"
#include "common_math.h"
typedef union mat4
{
    float ij[16];
    struct
    {
        float   a, b, c, d,
                e, f, g, h,
                i, j, k, l,
                m, n, o, p;
    };
}
mat4;
#define NEW_MATRIX(a, b, c, d, e, f, g, h, i, j, k, l, m, n, o, p) (mat4){\
a, b, c, d,\
e, f, g, h,\
i, j, k, l,\
m, n, o, p\
}
#define Ma 0
#define Mb 1
#define Mc 2
#define Md 3
#define Me 4
#define Mf 5
#define Mg 6
#define Mh 7
#define Mi 8
#define Mj 9
#define Mk 10
#define Ml 11
#define Mm 12
#define Mn 13
#define Mo 14
#define Mp 15
#define MDOT(ma, mb, j, i) (    ma[j*4+0]*mb[ 0+i] + \
                                ma[j*4+1]*mb[ 4+i] + \
                                ma[j*4+2]*mb[ 8+i] + \
                                ma[j*4+3]*mb[12+i])
#define MDOTV(ma, v, j) (       ma[j*4+0]*v[0] + \
                                ma[j*4+1]*v[1] + \
                                ma[j*4+2]*v[2] + \
                                ma[j*4+3]*v[3])

#define MATRIX_BASIC_OP(ma, mb, op) NEW_MATRIX(\
ma[0] op mb[0],    ma[1] op mb[1],    ma[2] op mb[2],    ma[3] op mb[3],    \
ma[4] op mb[4],    ma[5] op mb[5],    ma[6] op mb[6],    ma[7] op mb[7],    \
ma[8] op mb[8],    ma[9] op mb[9],    ma[10] op mb[10],  ma[11] op mb[11],  \
ma[12] op mb[12],  ma[13] op mb[13],  ma[14] op mb[14],  ma[15] op mb[15]   \
)
#define MATRIX_BASIC_SCALAR_OP(ma, s, op) NEW_MATRIX(\
ma[0] op s,     ma[1] op s,     ma[2] op s,     ma[3] op s, \
ma[4] op s,     ma[5] op s,     ma[6] op s,     ma[7] op s, \
ma[8] op s,     ma[9] op s,     ma[10] op s,    ma[11] op s,\
ma[12] op s,    ma[13] op s,    ma[14] op s,    ma[15] op s \
)

mat4 transpose(float *m)
{
    return NEW_MATRIX
    (
        m[Ma], m[Me], m[Mi], m[Mm],
        m[Mb], m[Mf], m[Mj], m[Mn],
        m[Mc], m[Mg], m[Mk], m[Mo],
        m[Md], m[Mh], m[Ml], m[Mp]
    );
}
mat4 madds(float *ma, float b)  { return MATRIX_BASIC_SCALAR_OP(ma, b, +); }
mat4 msubs(float *ma, float b)  { return MATRIX_BASIC_SCALAR_OP(ma, b, -); }
mat4 mmuls(float *ma, float b)  { return MATRIX_BASIC_SCALAR_OP(ma, b, *); }

mat4 madd(float *ma, float *mb) { return MATRIX_BASIC_OP(ma, mb, +); }
mat4 msub(float *ma, float *mb) { return MATRIX_BASIC_OP(ma, mb, -); }
mat4 mmul(float *ma, float *mb) 
{
    return NEW_MATRIX
    (
        MDOT(ma,mb,0,0),MDOT(ma,mb,0,1),MDOT(ma,mb,0,2),MDOT(ma,mb,0,3),
        MDOT(ma,mb,1,0),MDOT(ma,mb,1,1),MDOT(ma,mb,1,2),MDOT(ma,mb,1,3),
        MDOT(ma,mb,2,0),MDOT(ma,mb,2,1),MDOT(ma,mb,2,2),MDOT(ma,mb,2,3),
        MDOT(ma,mb,3,0),MDOT(ma,mb,3,1),MDOT(ma,mb,3,2),MDOT(ma,mb,3,3)
    );
}
vec4 mmulv(float *m, float *v) 
{
    return NEW_VECTOR
    (
        MDOTV(m,v,0),
        MDOTV(m,v,1),
        MDOTV(m,v,2),
        MDOTV(m,v,3)
    );
}

float det3x3(float *m)
{
    return  +m[Ma]*(m[Me]*m[Mi]-m[Mf]*m[Mh])
            -m[Mb]*(m[Md]*m[Mi]-m[Mf]*m[Mg])
            +m[Mc]*(m[Md]*m[Mh]-m[Me]*m[Mg]);
}
float det4x4(float *m)
{
    float   kplo = m[Mk]*m[Mp]-m[Ml]*m[Mo],
            jpln = m[Mj]*m[Mp]-m[Ml]*m[Mn],
            jokn = m[Mj]*m[Mo]-m[Mk]*m[Mn],
            iokm = m[Mi]*m[Mo]-m[Mk]*m[Mm],
            iplm = m[Mi]*m[Mp]-m[Ml]*m[Mm],
            injm = m[Mi]*m[Mn]-m[Mj]*m[Mm];

    return  +m[Ma]*(m[Mf]*(kplo)-m[Mg]*(jpln)+m[Mn]*(jokn))
            -m[Mb]*(m[Me]*(kplo)-m[Mg]*(iplm)+m[Mm]*(iokm))
            +m[Mc]*(m[Me]*(jpln)-m[Mf]*(iplm)+m[Mm]*(injm))
            -m[Md]*(m[Me]*(jokn)-m[Mf]*(iokm)+m[Mm]*(injm));
}
mat4 inverse(float *m) { return MATRIX_BASIC_SCALAR_OP(m, det4x4(m), *); }

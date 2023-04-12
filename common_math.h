#pragma once
#include "stdint.h"

#define PI          3.14159265359
#define _PI_2       1.57079632679
#define _PI_180     0.01745329251
#define _180_PI     57.2957795131

float max(float a, float b){return (a>b)?a:b;}
float min(float a, float b){return (a<b)?a:b;}
float clamp(float x, float l, float u){return min(max(x, l), u);}

float lerp(float a, float b, float t){return a+(b-a)*t;}

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
float q_rsqrt(float x)
{
    union {float f; uint32_t i;} c;
    c.f = x;
    c.i = 0x5F1FFFF9 - (c.i >> 1);
    return (c.f*0.703952253f*(2.38924456f-x*c.f*c.f));
}
float q_sqrt(float x){return x*q_rsqrt(x);}

#define Sc_max(a,b)     max(a,b)
#define Sc_min(a,b)     min(a,b)
#define Sc_clamp(x,l,u) clamp(x,l,u)
#define Sc_lerp(a,b,t)  lerp(a,b,t)

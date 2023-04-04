#pragma once
#define PI          3.14159265359
#define _PI_2       1.57079632679
#define _PI_180     0.01745329251
#define _180_PI     57.2957795131

float max(float a, float b){return (a>b)?a:b;}
float min(float a, float b){return (a<b)?a:b;}
float clamp(float x, float l, float u){return min(max(x, l), u);}

float lerp(float a, float b, float t){return a+(b-a)*t;}

#define Sc_max(a,b)     max(a,b)
#define Sc_min(a,b)     min(a,b)
#define Sc_clamp(x,l,u) clamp(x,l,u)
#define Sc_lerp(a,b,t)  lerp(a,b,t)

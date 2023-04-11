#pragma once
#include "transform.h"
#include "utils.h"

typedef struct camera
{
    float   FOV, ar;
    int     w, h;
    
    transform t;

    uint32_t *image;
}
camera;
camera new_camera(  int w, 
                    int h, 
                    float FOV, 
                    transform t,
                    uint32_t* image = nullptr)
{
    camera c = (camera)
    {
        .FOV=FOV,
        .ar=w/(float)h,
        .w=w,
        .h=h,
        .t=t,
        .image=(image == nullptr) ? alloc(w*h, uint32_t) : image
    };
    return c;
}

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdio>

#include <SDL2/SDL.h>
//#include <GL/glew.h>
//#include <GL/glu.h>

#include "vector.h"
#include "quaternion.h"
#include "matrix.h"

// Ray = O + t*D
typedef struct ray
{
    vec4    o,
            d;
    float   t;
}
ray;

vec4 at(ray &r)
{
    return NEW_VECTOR
    (
        r.o.xyzw[0] + r.t*r.d.xyzw[0], 
        r.o.xyzw[1] + r.t*r.d.xyzw[1], 
        r.o.xyzw[2] + r.t*r.d.xyzw[2], 
        0
    );
}

typedef struct camera
{
    float w, h, FOV, ar;
    uint32_t image;
}
camera;

typedef struct Display
{
    SDL_Window      *win;
    SDL_Texture     *tex;

    SDL_Renderer    *ren;

    size_t w, h;
    float ar;
}
Display;

int init_opengl()
{
    
}

int init_sdl()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return -1;

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
    
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  8);
    
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  8);
    

}

void render()
{

}

void write_img_to_ppm(uint32_t *img, const char * filepath)
{

}

//void draw_image()
//{
//    camera c = NEW_CAMERA();
//    render();
//    
//}

void test()
{
    quat q;
}

int main()
{
    return 0;
}
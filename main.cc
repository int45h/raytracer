#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <cstdio>

#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <GL/glu.h>

#include "vec4.h"

// Ray = O + t*D
typedef struct ray
{
    vec4    origin,
            hit;
    float   length;
}
ray;

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

int main()
{
    return 0;
}
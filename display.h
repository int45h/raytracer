#pragma once
#include <SDL2/SDL.h>

typedef struct Display
{
    SDL_Window      *win;

    SDL_Surface     *sur;
    SDL_Texture     *tex;
    SDL_Renderer    *ren;

    size_t w, h;
    float ar;
}
Display;

int init_sdl()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
        return -1;

    return 1;
}

Display new_display(int w, int h)
{
    Display d;
    d.w = w;
    d.h = h;

    if (SDL_CreateWindowAndRenderer
    (
        w, 
        h, 
        SDL_WINDOW_SHOWN, 
        &d.win, 
        &d.ren
    ) != 0)
        fprintf(stderr, "error: %s\n", SDL_GetError());
    d.sur = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA8888);
    //d.tex = SDL_CreateTextureFromSurface(d.ren, d.sur);
    return d;
}

void destroy_display(Display &d)
{
    if (d.tex != NULL)
        free(d.tex);
    if (d.ren != NULL)
        free(d.ren);
    if (d.win != NULL)
        free(d.win);
}

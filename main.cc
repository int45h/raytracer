#include <SDL2/SDL_error.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <cstdio>

#include <SDL2/SDL.h>
//#include <GL/glew.h>
//#include <GL/glu.h>

#include "vector.h"
#include "quaternion.h"
#include "matrix.h"

#include "common_gfx.h"

#define alloc(n,T)      (T*)malloc(n*sizeof(T))
#define copy(d,s,n,T)   memcpy((void*)d, (const void*)s, n*sizeof(T))

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
    float   FOV, ar;
    int     w, h;
    
    uint32_t *image;
}
camera;
camera new_camera(int w, int h, float FOV, uint32_t* image = nullptr)
{
    camera c = (camera)
    {
        .FOV=FOV,
        .ar=w/(float)h,
        .w=w,
        .h=h,
        .image=(image == nullptr) ? alloc(w*h, uint32_t) : image
    };
    return c;
}

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

uint32_t default_color(ray &r)
{
    vec4 color = vlerp
    (
        RGBA8888_TO_VECTOR(0x8cc6ff00).xyzw,
        RGBA8888_TO_VECTOR(0xe6f3ff00).xyzw,
        r.d.xyzw[Vy]
    );
    return VECTOR_TO_RGBA8888(color.xyzw);
}

uint32_t trace_ray(ray &r)
{
    return 0;
}

void render(camera &c)
{
    int spp     = 4,
        bounces = 4;

    for (int i = 0; i < c.w*c.h; i++)
    {
        vec4 color = NEW_VECTOR(
            ((float)(i%c.w))/(c.h*c.ar),
            ((float)i/(c.h*c.w)),
            0.25,
            1
        );

        c.image[i] = VECTOR_TO_RGBA8888(color.xyzw);

        //if ((i % c.h) == 0) printf("Progress: %d\n", 100*i/(c.w*c.h));
    }
}

void write_img_to_ppm(camera &c, const char *filepath)
{
    FILE *fp = fopen(filepath, "wb+");
    if (fp == NULL)
        return;
    
    fprintf(fp, "P3\n# %s\n%d %d\n255\n", filepath, c.w, c.h);
    for (int i = 0; i < c.w*c.h; i++)
    {
        fprintf(fp, "%d %d %d\n", CHANNEL(c.image[i], R),
                                  CHANNEL(c.image[i], G),
                                  CHANNEL(c.image[i], B));
    }

    fclose(fp);
    printf("Successfully saved file to %s!\n", filepath);
}

void draw_image()
{
    init_sdl();
    Display disp = new_display(640, 480);
    camera c = new_camera(disp.w, disp.h, 90, (uint32_t*)disp.sur->pixels);

    SDL_LockSurface(disp.sur);
    render(c);
    SDL_UnlockSurface(disp.sur);
    disp.tex = SDL_CreateTextureFromSurface(disp.ren, disp.sur);
    
    bool quit = false;
    SDL_Event e;
    do
    {
        SDL_RenderClear(disp.ren);
        SDL_RenderCopy(disp.ren, disp.tex, NULL, NULL);
        SDL_RenderPresent(disp.ren);
    
        if (!SDL_PollEvent(&e))
            continue;
        
        switch (e.type)
        {
            case SDL_QUIT:
                quit = true;
            case SDL_KEYUP:
                if (e.key.keysym.sym == SDLK_p)
                    write_img_to_ppm(c, "./out.ppm");
        }
    }
    while(!quit);

    destroy_display(disp);
}

void test()
{
    quat q;
}

int main()
{
    draw_image();
    return 0;
}
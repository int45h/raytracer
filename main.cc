#include <cstdio>
//#include <GL/glew.h>
//#include <GL/glu.h>

#include "transform.h"
#include "camera.h"

#include "common_gfx.h"
#include "utils.h"

#include "display.h"

// Ray = O + t*D
typedef struct ray
{
    vec4    o,
            d;
    float   t;
}
ray;

ray new_ray(vec4 origin, vec4 direction)
{
    ray r = (ray)
    {
        origin,
        direction,
        -1
    };

    return r;
}

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

uint32_t default_color(ray &r)
{
    vec4 color = vlerp
    (
        RGBA8888_TO_VECTOR(0x8cc6ff00).xyzw,
        RGBA8888_TO_VECTOR(0xe6f3ff00).xyzw,
        r.d.xyzw[Vy]
    );
    uint32_t out = VECTOR_TO_RGB888_32(color.xyzw);
    return out;
}

uint32_t trace_ray(ray &r)
{
    return default_color(r);
}

void render(camera &c, Display &disp)
{
    int spp     = 4,
        bounces = 4;

    float tan_fov = tan(c.FOV*0.5f);

    SDL_LockSurface(disp.sur);
    for (int i = 0; i < c.w*c.h; i++)
    {
        float   u = ((float)(i%c.w))/(c.h*c.ar),
                v = (((float)i)/(c.h*c.w));
        
        ray r = new_ray
        (
            VECTOR_ZERO, 
            vnorm(NEW_VECTOR(u, v*tan_fov, 1, 0).xyzw)
        );
        
        c.image[i] = trace_ray(r);
        //if ((i % c.h) == 0) printf("Progress: %d\n", 100*i/(c.w*c.h));
    }

    SDL_UnlockSurface(disp.sur);
    disp.tex = SDL_CreateTextureFromSurface(disp.ren, disp.sur);
}

void write_img_to_ppm(camera &c, const char *filepath)
{
    FILE *fp = fopen(filepath, "wb+");
    if (fp == NULL)
        return;
    
    fprintf(fp, "P3\n# %s\n%d %d\n255\n", filepath, c.w, c.h);
    for (int i = 0; i < c.w*c.h; i++)
    {
        fprintf(fp, "%d %d %d\n", CHANNEL(c.image[i], _R),
                                                    CHANNEL(c.image[i], _G),
                                                    CHANNEL(c.image[i], _B));
    }

    fclose(fp);
    printf("Successfully saved file to %s!\n", filepath);
}

void draw_image()
{
    init_sdl();
    Display disp    = new_display(640, 480);
    camera  c       = new_camera
    (
        disp.w, 
        disp.h, 
        90, 
        new_transform(), 
        (uint32_t*)disp.sur->pixels
    );

    render(c, disp);

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
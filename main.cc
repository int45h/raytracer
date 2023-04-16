#include <cstdint>
#include <cstdio>
#include <limits>
#include <vector>
//#include <GL/glew.h>
//#include <GL/glu.h>

#include "common_math.h"
#include "ray.h"
#include "camera.h"

#include "common_gfx.h"
#include "utils.h"

#include "display.h"
#include "vector.h"

typedef struct scene
{
    std::vector<ray_object> objects;
}
scene;

void destroy_scene(scene &s)
{
    for (ray_object &obj : s.objects)
        destroy_ray_object(obj);
    
    s.objects.clear();
}

scene build_test_scene()
{
    scene s;
    s.objects.push_back(new_sphere(new_transform(NEW_VECTOR(0, 0, 9, 0))));
    s.objects.push_back(new_sphere(new_transform(NEW_VECTOR(0, 3, 9, 0))));
    s.objects.push_back(new_sphere(new_transform(NEW_VECTOR(0, -3, 9, 0))));
    s.objects.push_back(new_sphere(new_transform(NEW_VECTOR(3, 0, 9, 0))));
    s.objects.push_back(new_sphere(new_transform(NEW_VECTOR(-3, 0, 9, 0))));
    s.objects.push_back(new_box(NEW_VECTOR(-1, -1, 3.1), NEW_VECTOR(0, 0, 6)));
    
    //ray_object t = new_triangle(alloc(9, float), alloc(3, int), new_transform());
    //t.vertices[0] = -0.5f; t.vertices[1] = -0.5f; t.vertices[2] = 1.f;
    //t.vertices[3] = +0.5f; t.vertices[4] = -0.5f; t.vertices[5] = 1.f;
    //t.vertices[6] = +0.0f; t.vertices[7] = +0.5f; t.vertices[8] = 1.f;
    //
    //t.indices[0] = 0; t.indices[1] = 1; t.indices[2] = 2;
    //s.objects.push_back(t);

    return s;
}

bool hit_object_in_scene(ray &r, scene &s, ray_object& out)
{
    // Simple distance based search
    float   closest_hit = r.t;
    bool    hit_found   = false;
    
    for (ray_object obj : s.objects)
    {
        if (!obj.hit(r)) 
            continue;
        
        if (closest_hit > r.t)
        {
            hit_found   = true;
            closest_hit = r.t;
            out         = obj;
        }
    }

    return hit_found;
}

uint32_t default_color(ray &r)
{
    float t = 0.5f*r.d.xyzw[Vy]+0.5f;
    vec4 color = vec4::vlerp_clamped
    (
        RGBA8888_TO_VECTOR(0xe6f3ff00),
        RGBA8888_TO_VECTOR(0x8cc6ff00),
        t
    );
    uint32_t out = VECTOR_TO_RGB888_32(color.xyzw);
    return out;
}

uint32_t trace_ray(ray &r, scene &s)
{
    ray_object obj = new_empty();
    if (!hit_object_in_scene(r, s, obj))
        return default_color(r);

    vec4 N = obj.norm(r);
    N = (N*0.5f)+0.5f;

    return VECTOR_TO_RGB888_32(N.xyzw);
}

void render(camera &c, Display &disp, scene &s)
{
    int spp     = 4,
        bounces = 4;

    float tan_fov = tan(c.FOV*0.5f*_PI_180);

    SDL_LockSurface(disp.sur);
    for (int i = 0; i < c.w*c.h; i++)
    {
        float   u = (float)(i%c.w),
                v = ((float)i)/c.w;
        
        ray r
        (
            VECTOR_ZERO, 
            vec4::norm(NEW_VECTOR(
                (2*(u+0.5f)/c.w-1)*tan_fov*c.ar, 
                (1-2*(v+0.5f)/c.h)*tan_fov, 
                1, 
                0
            ))
        );
        
        c.image[i] = trace_ray(r, s);
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
    scene s = build_test_scene();

    render(c, disp, s);

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

    destroy_scene(s);
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
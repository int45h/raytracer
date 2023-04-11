#pragma once
#include "transform.h"
#include "vector.h"

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

typedef enum geometry_type
{
    SPHERE,
    TRIANGLE,
    MESH
}
geometry_type;

typedef struct ray_object
{
    geometry_type   type;
    transform       t;

    float   *vertices;
    int     *indices;

    size_t vertex_size = 3; // vertex size in bytes, 3 for now
}
ray_object;

ray_object new_sphere(transform t, float radius = 1.f)
{
    ray_object r;

    r.type      = SPHERE;
    r.t         = t;
    r.t.scale   = NEW_VECTOR(radius,radius,radius,radius);

    return r;
}

ray_object new_triangle(float *vertices, int* indices, transform t)
{
    ray_object r;

    r.type      = TRIANGLE;
    r.vertices  = vertices;
    r.indices   = indices;

    return r;
}

void destroy_ray_object(ray_object &r)
{
    switch (r.type)
    {
        case TRIANGLE:
            free(r.vertices);
            free(r.indices);
    }
}

bool hit_sphere(ray &r, ray_object &obj)
{
    vec4    o_prime = vsub(r.o.xyzw, obj.t.position.xyzw);
    float   od      = vdot(o_prime.xyzw, r.d.xyzw),
            R       = obj.t.scale.xyzw[0],
            a       = vdot(r.d.xyzw, r.d.xyzw),
            b       = od,
            c       = vdot(o_prime.xyzw, o_prime.xyzw) - R*R,
            disc    = b*b - a*c;

    if (disc < 0)
        return false;
    
    float   sq_disc = sqrt(disc),
            t0      = (-b + sq_disc) / a,
            t1      = (-b - sq_disc) / a;

    if (t0 > 0 || t1 > 0)
    {
        if (t0 > t1) std::swap(t0, t1);
        
        r.t = t0;
        return true;
    }

    return false;
}

bool hit_triangle(ray &r, ray_object &obj)
{
    return false;
}

bool hit_object(ray &r, ray_object &obj)
{
    switch (obj.type)
    {
        case SPHERE:    return hit_sphere(r, obj);
        case TRIANGLE:  return hit_triangle(r, obj);
    }
    return false;
}

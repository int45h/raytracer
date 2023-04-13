#pragma once
#include "common_math.h"
#include "transform.h"
#include "vector.h"
#include <limits>

// Ray = O + t*D
typedef struct ray
{
    vec4    o,
            d;
    float   t;

    ray(const vec4& origin, const vec4& direction)
    {
        this->o = origin;
        this->d = direction;

        this->t = std::numeric_limits<float>::max();
    }

    inline vec4 at()
    {
        return NEW_VECTOR
        (
            this->o.xyzw[0] + this->t*this->d.xyzw[0], 
            this->o.xyzw[1] + this->t*this->d.xyzw[1], 
            this->o.xyzw[2] + this->t*this->d.xyzw[2], 
            0
        );
    }
}
ray;

typedef enum geometry_type
{
    NONE,
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

    inline vec4 vpos(int index)
    {
        return NEW_VECTOR
        (
            this->vertices[this->indices[index]*this->vertex_size+0],
            this->vertices[this->indices[index]*this->vertex_size+1],
            this->vertices[this->indices[index]*this->vertex_size+2]
        );
    }

    bool hit    (ray &r);
    vec4 norm   (ray &r);
}
ray_object;

ray_object new_empty()
{
    ray_object r;
    r.type = NONE;

    return r;
}

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
    vec4    o_prime = r.o - obj.t.position;
    float   od      = vec4::dot(o_prime, r.d),
            R       = obj.t.scale.xyzw[0],
            a       = vec4::length_sq(r.d),
            b       = od,
            c       = vec4::length_sq(o_prime) - R*R,
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

bool hit_triangle(ray &r, ray_object &obj, const vec4& v1, const vec4& v2, const vec4& v3)
{
    vec4    e1  = (v2 - v1),
            e2  = (v3 - v1),
            P   = vec4::cross(r.d, e2);
    float   det = vec4::dot(P, e1);
    
    // Test for parallel ray
    if (fabs(det) < FLOAT_EPSILON)
        return false;

    // Moller-Trumbore algorithm
    vec4    T   = r.o - v1,
            Q   = vec4::cross(T, e1);
    
    float   inv_det = 1 / det,
            t       = vec4::dot(e2,  Q)*inv_det,
            u       = vec4::dot(T,   P)*inv_det,
            v       = vec4::dot(r.d, Q)*inv_det;

    //printf("%f, %f, %f\n", t, u, v);

    if (u < FLOAT_EPSILON || u > 1)     return false;
    if (v < FLOAT_EPSILON || u+v > 1)   return false;

    r.t = t;
    return true;
}

vec4 sphere_norm(ray &r, ray_object &obj)   {return (obj.t.position - r.at()) / obj.t.scale.xyzw[0];}
vec4 triangle_norm(ray &r, ray_object &obj)
{
    vec4    v1 = obj.vpos(0), 
            v2 = obj.vpos(1),
            v3 = obj.vpos(2);

    return vec4::norm(vec4::cross(v2-v1,v3-v1));
}

bool ray_object::hit(ray &r)
{
    switch (this->type)
    {
        case SPHERE:    return hit_sphere(r, *this);
        case TRIANGLE:  
        {
            vec4    v1 = vpos(0),
                    v2 = vpos(1),
                    v3 = vpos(2);
            
            return hit_triangle(r, *this, v1, v2, v3);
        }
    }
    return false;
}

vec4 ray_object::norm(ray &r)
{
    switch (this->type)
    {
        case SPHERE:    return sphere_norm(r, *this);
        case TRIANGLE:  return triangle_norm(r, *this);
    }
    return VECTOR_ZERO;
}
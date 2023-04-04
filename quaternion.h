#pragma once
#include "vector.h"
#include "matrix.h"

#include "common_math.h"
typedef vec4 quat;

#define NEW_QUATERNION(w,x,y,z) NEW_VECTOR(x,y,z,w)
#define Qw 3
#define Qx 0
#define Qy 1
#define Qz 2

quat qnorm(float *q)
{
    float inv_len = vinvlen(q);
    return VECTOR_SCALAR_OP(q, inv_len, *);
}
quat qangle_axis(float *axis, float t)
{
    float t_2 = t * 0.5;
    return NEW_QUATERNION
    (
        (float)cos(t_2), 
        axis[0] * (float)sin(t_2), 
        axis[1] * (float)sin(t_2), 
        axis[2] * (float)sin(t_2)
    );
}
quat qangle_axis_norm(float *axis, float t)
{
    vec4 a = vnorm(axis);
    float t_2 = t * 0.5;
    return NEW_QUATERNION
    (
        (float)cos(t_2), 
        a.xyzw[0] * (float)sin(t_2), 
        a.xyzw[1] * (float)sin(t_2), 
        a.xyzw[2] * (float)sin(t_2)
    );
}

quat qadd(float *qa, float *qb){return VECTOR_BASIC_OP(qa, qb, +);}
quat qsub(float *qa, float *qb){return VECTOR_BASIC_OP(qa, qb, -);}
quat qmul(float *qa, float *qb)
{
    return NEW_QUATERNION
    (
        qa[Qw]*qb[Qw]-qa[Qx]*qb[Qx]-qa[Qy]*qb[Qy]-qa[Qz]*qb[Qz],
        qa[Qw]*qb[Qx]+qa[Qx]*qb[Qw]+qa[Qy]*qb[Qz]-qa[Qz]*qb[Qy],
        qa[Qw]*qb[Qy]-qa[Qx]*qb[Qz]+qa[Qy]*qb[Qw]+qa[Qz]*qb[Qx],
        qa[Qw]*qb[Qz]+qa[Qx]*qb[Qy]-qa[Qy]*qb[Qx]+qa[Qz]*qb[Qw]
    );
}
//https://eigen.tuxfamily.org/bz/show_bug.cgi?id=1779#:~:text=Here%20is%20the%20summary%3A%20The,which%20are%20a%20bit%20expensive.
vec4 qmulv(float *q, float *v)
{
    float t[] = {
        2*(q[Qy]*v[Vz]-q[Qz]*v[Vy]),
        2*(q[Qx]*v[Vz]-q[Qz]*v[Vx]),
        2*(q[Qx]*v[Vy]-q[Qy]*v[Vx]),
        0
    };

    return NEW_VECTOR
    (
        v[Vx]+q[Qw]*t[Vx]+(q[Qy]*t[Vz]-q[Qz]*t[Vy]),
        v[Vy]+q[Qw]*t[Vy]+(q[Qx]*t[Vz]-q[Qz]*t[Vx]),
        v[Vz]+q[Qw]*t[Vz]+(q[Qx]*t[Vy]-q[Qy]*t[Vx]),
        0
    );
}

//https://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles
mat4 qrot_matrix(float *q)
{
    float   qxqx = q[Qx]*q[Qx],
            qxqy = q[Qx]*q[Qy],
            qxqz = q[Qx]*q[Qz],
            qyqy = q[Qy]*q[Qy],
            qyqz = q[Qy]*q[Qz],
            qzqz = q[Qz]*q[Qz],
            qwqx = q[Qw]*q[Qx],
            qwqy = q[Qw]*q[Qy],
            qwqz = q[Qw]*q[Qz],
            qwqw = q[Qw]*q[Qw];
    
    return NEW_MATRIX
    (
        1-2*(qyqy+qzqz), 2*(qxqy-qwqz), 2*(qwqy+qxqz), 0, 
        2*(qxqy+qwqz), 1-2*(qxqx+qzqz), 2*(qyqz-qwqx), 0, 
        2*(qxqz-qwqy), 2*(qwqx+qyqz), 1-2*(qxqx+qyqy), 0, 
        0, 0, 0, 0
    );
}

quat qeuler(float *v)
{
    float   cr = cos(0.5f*v[Vx]),
            cp = cos(0.5f*v[Vy]),
            cy = cos(0.5f*v[Vz]),
            sr = sin(0.5f*v[Vx]),
            sp = sin(0.5f*v[Vy]),
            sy = sin(0.5f*v[Vz]),
            cpcy = cp*cy,
            spsy = sp*sy,
            cpsy = cp*sy,
            spcy = sp*cy;
    
    return NEW_QUATERNION
    (
        cr*cpcy+sr*spsy,
        sr*cpcy-cr*spsy,
        cr*spcy+sr*cpsy,
        cr*cpsy-sr*spcy
    );
}

vec4 eulerq(float *q)
{
    float   qxqx = q[Qx]*q[Qx],
            qxqy = q[Qx]*q[Qy],
            qyqy = q[Qy]*q[Qy],
            qyqz = q[Qy]*q[Qz],
            qzqz = q[Qz]*q[Qz],
            qwqx = q[Qw]*q[Qx],
            qwqz = q[Qw]*q[Qz],
            _2qwqyqxqz = 2*(q[Qw]*q[Qy]-q[Qx]*q[Qz]);
    
    return NEW_VECTOR
    (
        atan2(2*(qwqx+qyqz),1-2*(qxqx+qyqy)), 
        -_PI_2 + 2*(float)atan2(sqrt(1+_2qwqyqxqz),sqrt(1-_2qwqyqxqz)), 
        atan2(2*(qwqz+qxqy),1-2*(qyqy+qzqz)),
        0
    );
}
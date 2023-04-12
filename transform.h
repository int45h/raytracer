#pragma once
#include "matrix.h"
#include "vector.h"
#include "quaternion.h"

typedef struct transform
{
    mat4 TRS;

    vec4    position,
            scale;
    quat    rotation;
}
transform;

void rebuild_TRS(transform &t)
{
    mat4    R   =   qrot_matrix(t.rotation.xyzw),
            TS  =   NEW_MATRIX
                    (
                        t.scale.xyzw[0], 0, 0, t.position.xyzw[0], 
                        0, t.scale.xyzw[1], 0, t.position.xyzw[1], 
                        0, 0, t.scale.xyzw[2], t.position.xyzw[2], 
                        0, 0, 0, 1
                    );

    t.TRS = NEW_MATRIX
    (
        TS.ij[Ma]*R.ij[Ma]+TS.ij[Md]*R.ij[Mm], TS.ij[Ma]*R.ij[Mb]+TS.ij[Md]*R.ij[Mn], TS.ij[Ma]*R.ij[Mc]+TS.ij[Md]*R.ij[Mo], TS.ij[Ma]*R.ij[Md]+TS.ij[Md]*R.ij[Mp], 
        TS.ij[Mf]*R.ij[Ma]+TS.ij[Mh]*R.ij[Mm], TS.ij[Mf]*R.ij[Mb]+TS.ij[Mh]*R.ij[Mn], TS.ij[Mf]*R.ij[Mc]+TS.ij[Mh]*R.ij[Mo], TS.ij[Mf]*R.ij[Md]+TS.ij[Mh]*R.ij[Mp], 
        TS.ij[Mk]*R.ij[Ma]+TS.ij[Ml]*R.ij[Mm], TS.ij[Mk]*R.ij[Mb]+TS.ij[Ml]*R.ij[Mn], TS.ij[Mk]*R.ij[Mc]+TS.ij[Ml]*R.ij[Mo], TS.ij[Mk]*R.ij[Md]+TS.ij[Ml]*R.ij[Mp], 
        R.ij[Mm], R.ij[Mn], R.ij[Mo], R.ij[Mp]  
    );
}

transform new_transform(vec4 position = VECTOR_ZERO, quat rotation = QUATERNION_UNIT, vec4 scale = VECTOR_ZERO)
{
    transform t;
    t.rotation  = rotation;
    t.position  = position;
    t.scale     = scale;

    rebuild_TRS(t);
    return t;
}

void translate(vec4 deltaPos, transform &t)
{
    t.position += deltaPos;
    rebuild_TRS(t);
}

void rotate(quat rotation, transform &t)
{
    t.rotation = rotation;
    rebuild_TRS(t);
}

void scale(vec4 scale, transform &t)
{
    t.scale = scale;
    rebuild_TRS(t);
}
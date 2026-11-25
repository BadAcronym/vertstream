#ifndef VERTSTREAM_H
#define VERTSTREAM_H

#include "pd_int_types.h"
#include "pd_string_view.h"

typedef struct vs3DModel
{
    StringView name;
    f32        *vertices[3];
    // indices?
    // faces?
    // textures, normals, etc?
}
vs3DModel;

extern u08* vsReadObjPtr
(
    u08 *data
);

#endif

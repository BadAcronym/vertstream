#ifndef VERTSTREAM_H
#define VERTSTREAM_H

#include "pd_int_types.h"
#include "pd_string_view.h"

#define s_global     static
#define f_internal   static
#define v_persistent static

#define VS_OBJ_LINE_BUFSIZE 4096

typedef struct VSMesh
{
    f32 *vertices[4];
    f32 *spaces[3];
    u32 *indices[3];
    f32 *texCoords[3];
    f32 *normals[3];
}
VSMesh;

typedef struct VSModel
{
    StringView name;
    VSMesh     *meshes;
}
VSModel;

extern VSModel vsReadObj
(
    const char *filepath
);

#endif

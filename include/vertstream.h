#ifndef VERTSTREAM_H
#define VERTSTREAM_H

#include "pd_int_types.h"
#include "pd_string_view.h"

#define s_global     static
#define f_internal   static
#define v_persistent static

#define VS_OBJ_LINE_BUFSIZE 64

typedef struct VSVertex
{
    f32 x;
    f32 y;
    f32 z;
    f32 w;
}
VSVertex;

typedef struct VSTriple
{
    f32 x;
    f32 y;
    f32 z;
}
VSTriple;

typedef struct VSIndex
{
    u32 first;
    u32 second;
    u32 third;
}
VSIndex;

typedef struct VSMesh
{
    VSVertex *vertices;
    VSTriple *spaces;
    VSIndex  *indices;
    VSTriple *texCoords;
    VSTriple *normals;
}
VSMesh;

typedef struct VSModel
{
    StringView name;
    VSMesh     *meshes;
}
VSModel;

extern VSModel vsReadModel
(
    const char *filepath
);

extern VSModel vsReadObj
(
    StringView data
);

extern void vsFreeModel
(
    VSModel *model
);

#endif

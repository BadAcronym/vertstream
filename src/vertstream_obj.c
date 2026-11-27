#include "vertstream.h"
#include "vs_print_macros.h"

#include "pd_dyn_arr.h"

#include <inttypes.h>

s_global StringView faceIdent     = { .data = "f",  .size = 1 };
s_global StringView lineIdent     = { .data = "l",  .size = 1 };
s_global StringView vertexIdent   = { .data = "v",  .size = 1 };
s_global StringView spaceIdent    = { .data = "vp", .size = 2 };
s_global StringView normalIdent   = { .data = "vn", .size = 2 };
s_global StringView texCoordIdent = { .data = "vt", .size = 2 };

f_internal inline f32 readCoordinate
(
    u08    *cursor,
    String *line
){
    f32 coord = 0.0f;
    bool coordNegative = false;

    if(line->data[*cursor] == '-')
    {
        coordNegative = true;
        ++(*cursor);
    }

    if(line->data[*cursor] == '1')
    {
        coord += 1.0f;
    }

    *(cursor) += 2;
    float div = 1.0f;

    for(u08 i = 1; *cursor < line->size; ++(*cursor), ++i)
    {
        if(line->data[*cursor] == ' ')
        {
            break;
        }

        div *= 10.0f;

        // PERF: feels very inefficient. can't we just set the bits of the mantissa,
        // since we know the exponent is 0? (is it even 0 or 1?)
        // then we also don't have the need for xNegative and the deferred mult
        u08 digit = (u08)(line->data[*cursor]) - 0x30;
        coord += (f32)digit / (div);
    }

    if(coordNegative)
    {
        coord *= -1.0f;
    }

    ++(*cursor);
    return coord;
}

f_internal inline void readVertex
(
    VSMesh *mesh,
    String line
){
    VSVertex vertex = {0};
    vertex.w = 1.0f;

    u08 cursor = 2;
    vertex.x = readCoordinate(&cursor, &line);
    vertex.y = readCoordinate(&cursor, &line);
    vertex.z = readCoordinate(&cursor, &line);

    if(cursor < line.size)
    {
        vertex.w = readCoordinate(&cursor, &line);
    }

    VS_TRACE("read vertex: %f, %f, %f, %f", vertex.x, vertex.y, vertex.z, vertex.w);

    pdArrPush(mesh->vertices, vertex);
}

// PERF: think about parallelization. we should probably not dispatch a worker per-line.
// If none of the data needs to be ordered, we can simply dispatch a worker to work on n
// lines in parallel.
// obviously test around, especially with huge files that have an unnecessary amount of
// polygons.
VSModel vsReadObj
(
    StringView data
){
    VSModel obj = {0};

    char lineBuf[VS_OBJ_LINE_BUFSIZE];

    String line;
    line.data = lineBuf;
    line.size = VS_OBJ_LINE_BUFSIZE;

    u32 texCoordCount = 0;
    u32 normalCount   = 0;
    u32 spaceCount    = 0;
    u32 vertexCount   = 0;
    u32 lineCount     = 0;
    u32 faceCount     = 0;

    // NOTE: how many meshes can be in an .obj file?
    VSMesh mesh = {0};

    u64 cursor = 0;
    while(cursor < data.size)
    {
        pdSVReadLine(data, &cursor, &line, VS_OBJ_LINE_BUFSIZE);

        if(line.data[0] == '#')
        {
            continue;
        }

        StringView ident;
        ident.data = line.data;
        ident.size = 2;

        if(pdSVSame(ident, texCoordIdent))
        {
            ++texCoordCount;
            continue;
        }
        else if(pdSVSame(ident, normalIdent))
        {
            ++normalCount;
            continue;
        }

        ident.size = 1;
        if(pdSVSame(ident, spaceIdent))
        {
            ++spaceCount;
        }
        else if(pdSVSame(ident, vertexIdent))
        {
            ++vertexCount;
            readVertex(&mesh, line);
        }
        else if(pdSVSame(ident, lineIdent))
        {
            ++lineCount;
        }
        else if(pdSVSame(ident, faceIdent))
        {
            ++faceCount;
        }
    }

    // WIP: for now, while developing.
    VS_DEBUG("num. vertices:  %"PRIu32, vertexCount);
    VS_DEBUG("num. normals:   %"PRIu32, normalCount);
    VS_DEBUG("num. spaces:    %"PRIu32, spaceCount);
    VS_DEBUG("num. lines:     %"PRIu32, lineCount);
    VS_DEBUG("num. faces:     %"PRIu32, faceCount);
    VS_DEBUG("num. texCoords: %"PRIu32, texCoordCount);

    pdSVFree(&data);
    return obj;
}

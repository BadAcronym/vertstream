#include "vertstream.h"
#include "vs_print_macros.h"

#include <inttypes.h>

s_global StringView faceIdent     = { .data = "f",  .size = 1 };
s_global StringView lineIdent     = { .data = "l",  .size = 1 };
s_global StringView vertexIdent   = { .data = "v",  .size = 1 };
s_global StringView spaceIdent    = { .data = "vp", .size = 2 };
s_global StringView normalIdent   = { .data = "vn", .size = 2 };
s_global StringView texCoordIdent = { .data = "vt", .size = 2 };

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

    u64 cursor = 0;
    while(pdSVReadLine(data, &cursor, &line))
    {
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

    // WIP: for now, while developing
    VS_DEBUG("num. vertices:  %"PRIu32, vertexCount);
    VS_DEBUG("num. normals:   %"PRIu32, normalCount);
    VS_DEBUG("num. spaces:    %"PRIu32, spaceCount);
    VS_DEBUG("num. lines:     %"PRIu32, lineCount);
    VS_DEBUG("num. faces:     %"PRIu32, faceCount);
    VS_DEBUG("num. texCoords: %"PRIu32, texCoordCount);

    return obj;
}

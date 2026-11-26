#include "vertstream.h"

#include "pd_dyn_arr.h"

f_internal void vsFreeMesh
(
    VSMesh *mesh
){
    if(!mesh)
    {
        return;
    }

    if(mesh->vertices && pdArrSize(mesh->vertices))
    {
        pdArrFree(mesh->vertices);
    }

    if(mesh->spaces && pdArrSize(mesh->spaces))
    {
        pdArrFree(mesh->spaces);
    }

    if(mesh->indices && pdArrSize(mesh->indices))
    {
        pdArrFree(mesh->indices);
    }

    if(mesh->texCoords && pdArrSize(mesh->texCoords))
    {
        pdArrFree(mesh->texCoords);
    }

    if(mesh->normals && pdArrSize(mesh->normals))
    {
        pdArrFree(mesh->normals);
    }
}

void vsFreeModel
(
    VSModel *model
){
    if(!model)
    {
        return;
    }

    if(model->meshes)
    {
        for(u64 i = 0; i < pdArrSize(model->meshes); ++i)
        {
            vsFreeMesh(&model->meshes[i]);
        }
    }

    pdSVFree(&model->name);
}

VSModel vsReadModel
(
    const char *filepath
){
    // find out what type of model it is by file extension (or peeking)

    StringView data = pdSVReadFile(filepath);

    // for now, return obj always.
    return vsReadObj(data);
}

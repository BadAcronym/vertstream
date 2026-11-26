#include "vertstream.h"

#include "pd_int_types.h"

int main
(
    void
){
    u08 failed = 0;

    VSModel vase = vsReadModel("assets/WB_Vase.obj");

    vsFreeModel(&vase);

    return failed;
}

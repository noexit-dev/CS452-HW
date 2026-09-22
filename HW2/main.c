#include "balloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    Balloc pool = bcreate(2048,11,11);
    bprint(pool);

    void * ptr = balloc(pool, 2048);

    printf("\nPointer to First Allocation: %p\n", ptr);

    return 0;
}
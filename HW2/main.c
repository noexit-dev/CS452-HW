#include "balloc.h"
#include <stdio.h>
#include <stdlib.h>

int main(){
    Balloc pool = bcreate(2048,5,10);
    bprint(pool);

    void * ptr1 = balloc(pool, 2048);
    if (ptr1 == NULL){
        printf("\nballoc failed as intended, size request 2048 is too large for order 10 blocks\n");
    } else {
        printf("\nPointer to First Allocation: %p\n", ptr1);
        unsigned int size = bsize(pool, ptr1);
        printf("\nSize of First Allocation: %d\n", size);
    }

    void * ptr2 = balloc(pool, 1024);
    printf("\nPointer to Second Allocation: %p\n", ptr2);
    unsigned int size2 = bsize(pool, ptr2);
    printf("\nSize of Second Allocation: %d\n", size2);

    void * ptr3 = balloc(pool, 1024);
    printf("\nPointer to Third Allocation: %p\n", ptr3);
    unsigned int size3 = bsize(pool, ptr3);
    printf("\nSize of Third Allocation: %d\n", size3);

    bfree(pool, ptr2);
    bfree(pool, ptr3);

    return 0;
}
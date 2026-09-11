#include "balloc.h"
#include "bbm.h"
#include "utils.h"
#include <math.h>

// typedef struct BRep {
//     int l; //lowest order
//     int u; //highest (max) order
// } *BRep;  


//check if size is greater than u
//create a check comparing the size of the order to the size and see if you can break it up
//add metadata and freelist here maybe?
//upper arg is max order
extern Balloc bcreate(unsigned int size, int l, int u){
    //construct free list
    //set lower order value somewhere (maybe even upper value)
    return bbmcreate(size, u);
}

extern void bdelete(Balloc pool){
    bbmdelete(pool);
}

//allocates mem of size bytes from pool (bcreate)
extern void *balloc(Balloc pool, unsigned int size){
    if (size > bsize(pool, size)) {
        printf("Error: size request is larger than u arg");
        return NULL; //TODO: Check if this is right
    }
    //check order of size
    int order = size2e(size);

    //check free list

}

extern void  bfree(Balloc pool, void *mem){

}

extern unsigned int bsize(Balloc pool, void *mem){

}

extern void bprint(Balloc pool){

}
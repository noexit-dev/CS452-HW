#include "balloc.h"
#include "bbm.h"
#include "utils.h"
#include "freelist.h"
#include <math.h>


typedef struct {
    int lower_order;
    int upper_order;
    unsigned int poolsize;   
    void * baseaddr;
    BlockList blocklist;
    FreeList freelist;
    // blockmem; //what would block mem be? mmalloc
} *BRep; //balloc representation

typedef struct {
    unsigned int blocklist;
} *BlockList;


size_t metasize(int l, int u){
    //u-l creates size of freelist arr
    size_t metasize = 2 * sizeof(int) + sizeof(unsigned int) + sizeof(void *);

    //organize metadata as the following: l / u / poolsize / baseaddr (8 bytes) / blocklist (struct unsigned int arr of length l) / freelist (struct of ptr (8 bytes) and bitmap (char * which is going to be 2^l+1 / 8 to find amount of bytes))

}

extern Balloc bcreate(unsigned int size, int l, int u){
    //set lower order value somewhere (maybe even upper value)

    BRep brep; //create rep
    brep->lower_order = l;
    brep->upper_order = u;
    brep->poolsize = lower_power_of_2(size);
    brep->baseaddr = mmalloc(size);
    brep->freelist = freelistcreate(size, l, u); 
    // brep->blockmem =  //alloc size of block (round to floor pow of 2)
    
    //in order to get size of meta + pool i need size of meta
    
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
#include "balloc.h"
#include "bbm.h"
#include "utils.h"
#include "freelist.h"
#include <math.h>


typedef struct {
    void * baseaddr;
    int lower_order;
    int upper_order;
    unsigned int poolsize;   
    BlockList blocklist;
    FreeList freelist;
} *BRep; //balloc representation

typedef struct {
    unsigned int blocklist;
} *BlockList;

extern BlockList blocklistcreate(int l, unsigned int poolsize){
    

}
//organize metadata as the following: l / u / poolsize / baseaddr (8 bytes) / blocklist (struct unsigned int arr of length l) / freelist (struct of ptr (8 bytes) and bitmap (char * which is going to be 2^l+1 / 8 to find amount of bytes))
//u-l creates size of freelist arr

size_t metasize(int u, int l, unsigned int poolsize){
    size_t baseaddr_l_u_poolsize = sizeof(void *) + (sizeof(int) * 2) + sizeof(unsigned int);
    
    size_t head_ptrs_size = (u-l)* sizeof(void *);
    size_t bitmap_size = 0;
    for(int i = u; i < l; i--){
        size_t pairs = poolsize / e2size(i+1);
        bitmap_size += ceil(((double)pairs)/8);
    }
    size_t freelist_size = head_ptrs_size + bitmap_size;

    size_t blocklist_size = ((poolsize / e2size(l)) * 4);

    size_t metasize = baseaddr_l_u_poolsize + freelist_size + blocklist_size;

    return metasize;
}

extern Balloc bcreate(unsigned int size, int l, int u){
    //set lower order value somewhere (maybe even upper value)
    size_t meta = metasize(u,l,size);
    BlockList blocklist;

    BRep brep = mmalloc(meta+size); //alloc space for the struct
    brep->baseaddr = brep+meta; //find with offset of metasize maybe 
    brep->lower_order = l;
    brep->upper_order = u;
    brep->poolsize = lower_power_of_2(size);
    brep->freelist = freelistcreate(size, l, u);
    brep->blocklist = blocklist;
    //in order to get size of meta + pool i need size of meta

    brep->blocklist->;


    
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
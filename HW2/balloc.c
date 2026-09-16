#include <math.h>
#include "balloc.h"
#include "bbm.h"
#include "utils.h"
#include "freelist.h"

typedef struct {
    int lower_order;
    int upper_order;
    unsigned int poolsize;  
} *Metadata;

typedef struct {
    void * baseaddr;
    Metadata * metadataaddr;
    // FreeList * freelistaddr;
} *BRep; //balloc representation

BRep brep;

size_t metasize(int u, int l, unsigned int poolsize){
    size_t baseaddr_l_u_poolsize = sizeof(void *) + (sizeof(int) * 2) + sizeof(unsigned int);
    size_t blocklist_size = ((poolsize / e2size(l)) * 4);
    size_t metasize = baseaddr_l_u_poolsize + blocklist_size;
    return metasize;
}

extern Balloc bcreate(unsigned int size, int l, int u){
    //create poolsize rounding to lowest power of 2
    unsigned int poolsize = higher_power_of_2(size);
    
    //allocating data
    Metadata * metadata = mmalloc(metasize(u,l,size));
    // FreeList * freelist = freelistcreate(size, l, u);
    
    Balloc * pool = mmalloc(poolsize);
    
    //storing metadata
    brep->metadataaddr = metadata;
    (*brep->metadataaddr)->lower_order = l;
    (*brep->metadataaddr)->upper_order = u;
    (*brep->metadataaddr)->poolsize = poolsize;
    brep->baseaddr = pool;

    // brep->freelistaddr = freelist;
    // ((FreeListElement )(brep->freelistaddr))[u-l]->head = pool;
    // (*brep->freelistaddr)[u-l]->freebm = bbmcreate(poolsize,u-l);

    return (Balloc) brep;
}

extern void bdelete(Balloc pool){
    
}

//allocates mem of size bytes from pool (bcreate)
extern void * balloc(Balloc pool, unsigned int size){
    // if (size > bsize(pool, size)) {
    //     printf("Error: size request is larger than u arg");
    //     return NULL; //TODO: Check if this is right
    // }

    //check order of size
    // order = size2e(size);
    void *mock_ptr = (void *)0x1000;
    
    return mock_ptr;
}

extern void  bfree(Balloc pool, void *mem){

}

extern unsigned int bsize(Balloc pool, void *mem){
    return 1;
}

extern void bprint(Balloc pool){

}
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
    Metadata metadataaddr;
    void * baseaddr;
    FreeList freelistaddr;
} *BRep; //balloc representation


size_t metasize(){
    size_t metasize = (sizeof(int) * 2) + sizeof(unsigned int);
    return metasize;
}

extern Balloc bcreate(unsigned int size, int l, int u){ // 2^3 is the lowest possible order you can go
    BRep brep;

    //create poolsize rounding to lowest power of 2
    unsigned int poolsize = higher_power_of_2(size);
    int freelist_arr_length = u-l;
    char * freebm = bbmcreate(poolsize,freelist_arr_length);
    
    //allocating Balloc Representation TODO add freelist
    brep = mmalloc(sizeof(struct { Metadata metadataaddr; void * baseaddr; FreeList * freelistaddr;}));
    
    //allocating metadata struct, freelist, and pool
    Metadata metadata = mmalloc(sizeof(*metadata));
    FreeList freelist = freelistcreate(poolsize, l, u); //mmap puts it at the highest possible address space? TODO: ask buff.
    void * pool = mmalloc(poolsize);
    
    // //storing metadata
    brep->metadataaddr = metadata;
    metadata->lower_order = l;
    metadata->upper_order = u;
    metadata->poolsize = poolsize;
    brep->baseaddr = pool;

    brep->freelistaddr = freelist;
    freelist_set_head((FreeList)brep->freelistaddr, freelist_arr_length, brep->baseaddr);
    freelist_set_bm((FreeList)brep->freelistaddr, freelist_arr_length, freebm);

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
    int order = size2e(size);

    FreeList freelist = ((BRep)pool)->freelistaddr;
    Metadata meta = ((BRep)pool)->metadataaddr;
    int freelist_length = meta->upper_order - meta->lower_order;
    

    
    //for freelist at order list go until you find freeblock
    
    for(int i = meta->lower_order; i < meta->upper_order; i++){
        //start at lower order
        if (){
            
        }

    }

}

extern void  bfree(Balloc pool, void *mem){

}

extern unsigned int bsize(Balloc pool, void *mem){
    
}

//this function will not work if wrapper class is in hw directory
extern void bprint(Balloc pool){
     if (pool == NULL) {
        printf("Error: bprint called with a NULL Balloc pool.\n");
        return;
    }

    void * base = ((BRep)pool)->baseaddr;
    Metadata meta = ((BRep)pool)->metadataaddr;
    FreeList freelist = ((BRep)pool)->freelistaddr;

    printf("Metadata Address: %p \n", meta);
    printf("- Lower Order: %d \n", meta->lower_order);
    printf("- Upper Order: %d \n", meta->upper_order);
    printf("- Pool Size: %u \n", meta->poolsize);
    
    freelistprint(freelist, meta->lower_order, meta->upper_order);
    
    printf("Base Address: %p \n", base);

}
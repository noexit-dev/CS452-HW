#include <math.h>
#include "balloc.h"
#include "bbm.h"
#include "utils.h"
#include "freelist.h"

/**
 * @brief metadata representation
 * 
 */
typedef struct {
    int lower_order;
    int upper_order;
    unsigned int poolsize;  
} *Metadata;

/**
 * @brief balloc representation
 * 
 */
typedef struct {
    Metadata metadataaddr;
    void * baseaddr;
    FreeList freelistaddr;
} *BRep;

/**
 * @brief creates a Buddy Allocator
 * 
 * @param size - size of Allocator
 * @param l - lowest order of Allocator (lowest can be 2^3, which is size of pointer)
 * @param u - highest order of Allocator
 * @return Balloc - returns pointer to Buddy Allocator or NULL/0 if failed
 */
extern Balloc bcreate(unsigned int size, int l, int u){
    BRep brep;

    //create poolsize rounding to lowest power of 2
    unsigned int poolsize = higher_power_of_2(size);
    int freelist_arr_length = u-l;
    
    //allocating Balloc Representation
    brep = mmalloc(sizeof(struct { Metadata metadataaddr; void * baseaddr; FreeList * freelistaddr;}));
    
    //allocating metadata struct, freelist, and pool
    Metadata metadata = mmalloc(sizeof(*metadata));
    FreeList freelist = freelistcreate(poolsize, l, u);
    void * pool = mmalloc(poolsize);
    
    //storing metadata
    brep->metadataaddr = metadata;
    metadata->lower_order = l;
    metadata->upper_order = u;
    metadata->poolsize = poolsize;
    brep->baseaddr = pool;
    brep->freelistaddr = freelist;
    
    if (size2e(poolsize) > u){
        int num_of_buddies = poolsize / e2size(u);

        for (int i = 0; i < num_of_buddies; i++){
            push_free_block(freelist, freelist_arr_length, (pool + (e2size(u) * i)));
        }
    } else {
        freelist_set_head(freelist, freelist_arr_length, brep->baseaddr);
    }

    return (Balloc) brep;
}

/**
 * @brief deletes/frees the Buddy Allocator
 * 
 * @param pool - pointer to the Buddy Allocator
 */
extern void bdelete(Balloc pool){
    mmfree(pool, sizeof(struct { Metadata metadataaddr; void * baseaddr; FreeList * freelistaddr;}));
}

/**
 * @brief Allocates memory from the Buddy Allocator based on size request
 * 
 * @param pool - pointer to Buddy Allocator
 * @param size - size request
 * @return void* - pointer to allocation
 */
extern void * balloc(Balloc pool, unsigned int size){
    Metadata meta = ((BRep)pool)->metadataaddr;

    //check order of size
    int order = size2e(size);
    if (order > meta->upper_order){
        return NULL;
    }

    if (order < meta->lower_order){
        order = meta->lower_order;
    }

    FreeList freelist = ((BRep)pool)->freelistaddr;
    
    return freelistalloc(freelist, ((BRep)pool)->baseaddr, order, meta->lower_order, meta->upper_order);
}

/**
 * @brief frees an allocation from the Buddy Allocator
 * 
 * @param pool - pointer to Buddy Allocator
 * @param mem - pointer to memory address to be freed
 */
extern void  bfree(Balloc pool, void *mem){
    Metadata meta = ((BRep)pool)->metadataaddr;
    FreeList freelist = ((BRep)pool)->freelistaddr;
    
    freelistfree(freelist, ((BRep)pool)->baseaddr, mem, meta->upper_order, meta->lower_order);
}

/**
 * @brief returns size of an allocation
 * 
 * @param pool - pointer to Buddy Allocator
 * @param mem - pointer to memory address of allocation
 * @return unsigned int - size of allocation in bytes
 */
extern unsigned int bsize(Balloc pool, void *mem){
    Metadata meta = ((BRep)pool)->metadataaddr;
    FreeList freelist = ((BRep)pool)->freelistaddr;

    // check each bm from l all the way to u until you find a bit that is allocated
    //then you return the size at that order
    return freelistsize(freelist, ((BRep)pool)->baseaddr, mem, meta->lower_order, meta->upper_order);
}

/**
 * @brief prints Buddy Allocator information
 * 
 * @param pool - pointer to Buddy Allocator
 */
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
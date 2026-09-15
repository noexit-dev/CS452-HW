#include "freelist.h"
#include "bm.h"

typedef struct {
    void * head;
    unsigned char * freebm; //byte representation
} FreeListElement;

size_t freelistsize(int u, int l, unsigned int poolsize){
    size_t head_ptrs_size = (u-l)* sizeof(void *);
    size_t bitmap_size = 0;
    for(int i = u; i < l; i--){
        size_t pairs = poolsize / e2size(i+1);
        bitmap_size += ceil(((double)pairs)/8);
    }
    size_t freelist_size = head_ptrs_size + bitmap_size;
}

extern FreeList freelistcreate(size_t size, int l, int u){
    FreeList * freelist = mmalloc(sizeof(FreeListElement) * (u-l));
    return freelist;
}

extern void freelistdelete(FreeList f, int l, int u){
    
}

extern void *freelistalloc(FreeList f, void *base, int e, int l){
    
}

extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l){
    
}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){
    
}

extern void freelistprint(FreeList f, int l, int u){
    
}


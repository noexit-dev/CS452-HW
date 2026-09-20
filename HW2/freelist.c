#include "freelist.h"
#include "bm.h"
#include "utils.h"

typedef struct {
    void * head;
    char * freebm; //byte representation
} FreeListElement;

// size_t freelistsize(int u, int l, unsigned int poolsize){
//     size_t head_ptrs_size = (u-l)* sizeof(void *);
//     size_t bitmap_size = 0;
//     for(int i = u; i < l; i--){
//         size_t pairs = poolsize / e2size(i+1);
//         bitmap_size += ceil(((double)pairs)/8);
//     }
//     size_t freelist_size = head_ptrs_size + bitmap_size;
// }

extern FreeList freelistcreate(size_t size, int l, int u){
    FreeList * freelist = mmalloc(sizeof(FreeListElement) * (u-l + 1));
    return freelist;
} //TODO add error handling for creation of freelist

extern void freelist_set_head(FreeList f, int idx, void *head){
    ((FreeListElement *)f)[idx].head = head;
}

extern void freelist_set_bm(FreeList f, int idx, char * freebm){
    ((FreeListElement *)f)[idx].freebm = freebm;
}

extern void freelistdelete(FreeList f, int l, int u){
    
}

extern void *freelistalloc(FreeList f, void *base, int e, int l){
    if(!((FreeListElement *)f)[e].head){
        return NULL;
    } else {

        
    }



}

extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l){
    
}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){
    return 1;
}

extern void freelistprint(FreeList f, int l, int u){
    if (f == NULL) return;

    printf("FreeList Address: %p \n", f);
    printf("- Lower Order: %d \n", l);
    printf("- Upper Order: %d \n", u);
}


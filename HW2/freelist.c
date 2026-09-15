#include "freelist.h"
#include "bm.h"

// typedef struct free_list {
//     void * head;
//     unsigned char * freebm; //byte representation
// } *FL;

//uses linked list (deq?) stores a linked list of free page blocks
// and a bit map representing the free and used blocks
extern FreeList freelistcreate(size_t size, int l, int u){
    // FL freelist[u-l];
    

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


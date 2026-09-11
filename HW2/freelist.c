#include "freelist.h"

typedef struct Node {
    struct Node * next;        // next/prev neighbors
    void * ptrToFreeSpace;
} *Node;

typedef struct FL {
    Node * head;
    unsigned long * map;
} *FL;

//uses linked list (deq?) stores a linked list of free page blocks
// and a bit map representing the free and used blocks
extern FreeList freelistcreate(size_t size, int l, int u){
    int arr_size = u-l;
    FL free_area[arr_size];
    for (int i = 0; i < arr_size; i++){
        free_area
    }
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


#include "freelist.h"

typedef struct Node {
    struct Node * next;
    void * ptrToFreeSpace;
} *Node;

typedef struct FL {
    Node * head;
    unsigned long * map;
} *FL;

typedef struct OL {
    int Max_Order;
    int Min_Order;
    struct FL* free_lists[];
} *OL;


//uses linked list (deq?) stores a linked list of free page blocks
// and a bit map representing the free and used blocks
extern FreeList freelistcreate(size_t size, int l, int u){
    //create head reference for max order block
    OL orderlist;
    orderlist->Max_Order = u;
    orderlist->Min_Order = l;
    // orderlist->free_lists[u-(l++)]; //u - (l++) upper - lower with an offset of 1 for the array size handles the case where upper and lower are the same and should still have a list for each order
    return (FreeList)orderlist;
}

extern void freelistdelete(FreeList f, int l, int u){

}

extern void *freelistalloc(FreeList f, void *base, int e, int l){
    (OL)f->free_lists[];
    
}

extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l){

}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){

}

extern void freelistprint(FreeList f, int l, int u){

}


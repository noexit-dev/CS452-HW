#include "freelist.h"
#include "bbm.h"
#include "utils.h"

typedef struct {
    void * head;
    char * freebm; //byte representation
} FreeListElement;

void push_free_block(FreeList f, int order, void * new_block){
    FreeListElement* free_lists = (FreeListElement*)f;

    if (new_block == NULL) return;

    *(void**)new_block = free_lists[order].head;
    free_lists[order].head = new_block;
};

void *pop_free_block(FreeList f,  int order) {
    FreeListElement* free_lists = (FreeListElement*)f;

    void* block_to_allocate = free_lists[order].head;
    
    if (block_to_allocate == NULL) {
        return NULL;
    }

    void* next_block = *(void**)block_to_allocate;

    free_lists[order].head = next_block;

    return block_to_allocate;
}

void split_block(FreeList f, int current_order, int target_order){
    //split logic
    int curr = current_order;

    while (curr != target_order){ //split down one order until target order
        void* buddy1 = pop_free_block(f, curr);
        if (buddy1 == NULL) return;

        int inter = curr - 1; //intermediate
        size_t buddy_size = e2size(inter);

        void* buddy2 = (void*)(buddy1 + buddy_size);

        push_free_block(f, target_order, buddy2);
        push_free_block(f, target_order, buddy1);

        curr--;
    }
}

extern FreeList freelistcreate(size_t size, int l, int u){ //push buddies
    FreeList * freelist = mmalloc(sizeof(FreeListElement) * (u-l + 1));
    return freelist;
} //TODO add error handling for creation of freelist

extern void freelist_set_head(FreeList f, int idx, void *head){
    ((FreeListElement *)f)[idx].head = head;
}

extern void freelist_set_bm(FreeList f, int idx, char * freebm){
    ((FreeListElement *)f)[idx].freebm = freebm;
}

//freeing freelist for allocator 
extern void freelistdelete(FreeList f, int l, int u){
    mmfree(f, sizeof(FreeListElement) * (u-l + 1));
}

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){ //e target order //u is the highest order to take from 
    FreeListElement* free_lists = (FreeListElement*)f;
    void * freeblock; //the block to be allocated

    int target_order = e-l;
    int current_order = e-l;

    if (free_lists[current_order].head == NULL){ //is an exact-size block free? no, so split buddies
        while(free_lists[current_order].head == NULL){ //find order that has free blocks
            if (current_order > u){
                break;
            }
            current_order++;
        }
        split_block(f, current_order, target_order);
    
    }
    
    freeblock = free_lists[current_order].head;

    if (free_lists[current_order].head != NULL && *(void**)(free_lists[current_order].head) != NULL) { //if there is a next pointer set head to what is next
        free_lists[current_order].head = *(void**)(free_lists[current_order].head);
    } else { //if there isn't a next pointer set head to null
        free_lists[current_order].head = NULL;
    }

    return freeblock;
}

//freeing an allocation in the allocator by putting it back on the freelist and coalescing
extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l){
    
}

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){
    FreeListElement * free_list = (FreeListElement *)f;
    int current_order = l;
    int found = 0;

    while (found == 0){
        if (bbmtst(free_list[current_order].freebm, base, mem, current_order) == 1){ //TODO: bmbits has weird thing happening
            found++;
        } else {
            if (current_order > u){
                break;
            }
            current_order++;
        }    
    }

    if (found == 0){
        return 0;
    }
    return e2size(current_order);
}

extern void freelistprint(FreeList f, int l, int u){
    if (f == NULL) return;

    printf("FreeList Address: %p \n", f);
    printf("- Lower Order: %d \n", l);
    printf("- Upper Order: %d \n", u);
}


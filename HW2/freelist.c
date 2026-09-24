#include "freelist.h"
#include "bbm.h"
#include "utils.h"

typedef struct {
    void * head;
    char * freebm; //byte representation
} FreeListElement;

extern void push_free_block(FreeList f, int order, void * new_block){
    FreeListElement* free_lists = (FreeListElement*)f;

    if (new_block == NULL) return;

    *(void**)new_block = free_lists[order].head;
    free_lists[order].head = new_block;
}

extern void *pop_free_block(FreeList f,  int order) {
    FreeListElement* free_lists = (FreeListElement*)f;

    void* block_to_allocate = free_lists[order].head;
    
    if (block_to_allocate == NULL) {
        return NULL;
    }

    void* next_block = *(void**)block_to_allocate;

    free_lists[order].head = next_block;

    return block_to_allocate;
}

extern void split_block(FreeList f, int current_order, int target_order, int l){
    // Split down level by level from current_order to target_order using absolute orders
    void* buddy1;

    while (current_order > target_order) {
        buddy1 = pop_free_block(f, current_order);
        if (buddy1 == NULL) return;

        current_order--;
        size_t buddy_size = e2size(current_order + l);
        void* buddy2 = (void*)(buddy1 + buddy_size);

        push_free_block(f, current_order, buddy1);
        push_free_block(f, current_order, buddy2);
    }


}

extern FreeList freelistcreate(size_t size, int l, int u){
    FreeListElement * freelist = mmalloc(sizeof(FreeListElement) * (u - l + 1));
    if (freelist == NULL) return NULL;

    for (int i = 0; i < (u - l + 1); i++) {
        freelist[i].head = NULL;
        freelist[i].freebm = bbmcreate(size, l + i);
    }

    return (FreeList) freelist;
}

extern void freelist_set_head(FreeList f, int idx, void *head){
    if (f == NULL) return;
    ((FreeListElement *)f)[idx].head = head;
}

extern void freelist_set_bm(FreeList f, int idx, char * freebm){
    if (f == NULL) return;
    ((FreeListElement *)f)[idx].freebm = freebm;
}

extern void freelistdelete(FreeList f, int l, int u){
    if (f == NULL) return;
    int total_orders = (u - l) + 1;
    for (int i = 0; i < total_orders; i++) {
        bbmdelete(((FreeListElement *)f)[i].freebm);
    }
    mmfree(f, sizeof(FreeListElement) * total_orders);
}

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){ 
    FreeListElement* free_lists = (FreeListElement*)f;
    void * freeblock; 

    int target_order = e - l;
    int current_order = target_order;

    if (free_lists[current_order].head == NULL){ 
        while(current_order <= u - l && free_lists[current_order].head == NULL){ 
            current_order++;
        }
        if (current_order > u - l) {
            return NULL;
        }
        split_block(f, current_order, target_order, l);
    }
    
    freeblock = free_lists[target_order].head;

    if (free_lists[target_order].head != NULL && *(void**)(free_lists[target_order].head) != NULL) { 
        free_lists[target_order].head = *(void**)(free_lists[target_order].head);
    } else { 
        free_lists[target_order].head = NULL;
    }

    // Use absolute order (target_order + l) for the bitmap
    bbmset(free_lists[target_order].freebm, base, freeblock, e);

    return freeblock;
}

extern void freelistfree(FreeList f, void *base, void *mem, int u, int l){
    FreeListElement * free_list = (FreeListElement *)f;
    int current_order = 0;
    int max_order = u - l;

    // Find the allocation order using absolute order (current_order + l)
    while (current_order <= max_order){
        if (bbmtst(free_list[current_order].freebm, base, mem, current_order + l) == 1){ 
            break;
        }
        current_order++;
    }
     
    if (current_order > max_order){
        return;
    }

    // Clear the allocation bit at the found order
    bbmclr(free_list[current_order].freebm, base, mem, current_order + l);

    void * curr_block = mem;
    
    // Coalesce with free buddies upwards
    while (current_order < max_order) {
        void *buddy = baddrinv(base, curr_block, current_order + l);
        
        void *prev = NULL;
        void *curr = free_list[current_order].head;
        int buddy_found = 0;

        while (curr != NULL) {
            if (curr == buddy) {
                buddy_found = 1;
                break;
            }
            prev = curr;
            curr = *(void **)curr; 
        }

        if (!buddy_found) {
            break; 
        }

        if (prev == NULL) {
            free_list[current_order].head = *(void **)buddy;
        } else {
            *(void **)prev = *(void **)buddy;
        }

        if (buddy < curr_block) {
            curr_block = buddy;
        }

        current_order++;
    }

    push_free_block(free_list, current_order, curr_block);
}    

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u){
    FreeListElement * free_list = (FreeListElement *)f;
    int current_order = 0;
    int max_order = u - l;
    int found = 0;

    // Search upwards from lowest order to find where the block is marked allocated
    while (current_order <= max_order){
        if (bbmtst(free_list[current_order].freebm, base, mem, current_order + l) == 1){ 
            found = 1;
            break;
        }    
        current_order++;
    }

    if (found == 0){
        return 0;
    }
    return e2size(current_order + l);
}

extern void freelistprint(FreeList f, int l, int u){
    if (f == NULL) return;
    FreeListElement * freelist = (FreeListElement*)f;

    printf("FreeList Address: %p \n", f);
    printf("- Lower Order: %d \n", l);
    printf("- Upper Order: %d \n", u);

    for (int i = 0; i < u-l+1; i++){
        printf("Head addr at order %d : %p\n", i+l, freelist[i].head);
        printf("BBM addr at order %d : %p\n", i+l, freelist[i].freebm);
    }
    // printf("BBM at order %d : %s\n", 11, *(char**)freelist[11].freebm);
    
}
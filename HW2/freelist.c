#include "freelist.h"
#include "utils.h"
#include "bbm.h"

typedef struct {
    void * head; //pointer to head of freeblock
    char * freebm; //byte representation
} FreeListElement;

/**
 * @brief Pushes free block to a certain order on the free list.
 * 
 * @param f - pointer to freelist
 * @param order - order of block to be pushed at
 * @param new_block - pointer to the new free block in the pool
 */
extern void push_free_block(FreeList f, int order, void * new_block){
    FreeListElement* free_lists = (FreeListElement*)f;

    if (new_block == NULL) return;

    *(void**)new_block = free_lists[order].head;
    free_lists[order].head = new_block;
}

/**
 * @brief Pops free block of a certain order on the free list
 * 
 * @param f - pointer to freelist
 * @param order - order of block to be popped off
 * @return void* - pointer to free block to allocate
 */
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

/**
 * @brief splits the block of a certain order
 * 
 * @param f - pointer to freelist
 * @param current_order - order where free blocks are
 * @param target_order - order of blocks we are trying to get
 * @param l - lower order from Buddy Allocator
 */
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

/**
 * @brief creates/allocates the freelist and returns a pointer to the start of the list
 * 
 * @param size - size of memory pool
 * @param l - lower order from Buddy Allocator
 * @param u - upper order from Buddy Allocator
 * @return FreeList - pointer to freelist
 */
extern FreeList freelistcreate(size_t size, int l, int u){
    FreeListElement * freelist = mmalloc(sizeof(FreeListElement) * (u - l + 1));
    if (freelist == NULL) return NULL;

    for (int i = 0; i < (u - l + 1); i++) {
        freelist[i].head = NULL;
        freelist[i].freebm = bbmcreate(size, l + i);
    }

    return (FreeList) freelist;
}

/**
 * @brief sets the head pointer for a specified index/order on the freelist
 * 
 * @param f - pointer to the freelist
 * @param idx - array index on the freelist
 * @param head - head pointer to be set
 */
extern void freelist_set_head(FreeList f, int idx, void *head){
    if (f == NULL) return;
    ((FreeListElement *)f)[idx].head = head;
}

/**
 * @brief deletes/deallocates the freelist specified
 * 
 * @param f - pointer to the freelist to be deleted/deallocated
 * @param l - lower order from the Buddy Allocator
 * @param u - upper order from the Buddy Allocator
 */
extern void freelistdelete(FreeList f, int l, int u){
    if (f == NULL) return;
    int total_orders = (u - l) + 1;
    for (int i = 0; i < total_orders; i++) {
        bbmdelete(((FreeListElement *)f)[i].freebm);
    }
    mmfree(f, sizeof(FreeListElement) * total_orders);
}

/**
 * @brief provides a free block at a desired order
 * 
 * @param f - pointer to the freelist
 * @param base - base address of memory pool from the Buddy Allocator
 * @param e - order of request size
 * @param l - lower order from the Buddy Allocator
 * @param u - upper order from the Buddy Allocator
 * @return void* - pointer to the block of memory to be allocated or NULL if no free block can be allocated
 */
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

/**
 * @brief frees a block and adds it to the free list. This also coalesces buddy blocks if both are free to create free blocks for bigger requests
 * 
 * @param f - pointer to the freelist
 * @param base - base address of memory pool from the Buddy Allocator
 * @param mem - memory address of allocated block to be freed
 * @param u - upper order from the Buddy Allocator
 * @param l - lower order from the Buddy Allocator
 */
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

/**
 * @brief returns the size of an allocation (not the request) from the memory pool
 * 
 * @param f - pointer to the freelist
 * @param base - base address of memory pool from the Buddy Allocator
 * @param mem - memory address of allocated block to be freed
 * @param l - lower order from the Buddy Allocator
 * @param u - upper order from the Buddy Allocator
 * @return int - size of allocation as an integer or NULL if allocation is not found in the freelist
 */
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

/**
 * @brief prints information about the freelist. Used for debugging
 * 
 * @param f - pointer to the freelist
 * @param l - lower order from the Buddy Allocator
 * @param u - upper order from the Buddy Allocator
 */
extern void freelistprint(FreeList f, int l, int u){
    if (f == NULL) return;
    // FreeListElement * freelist = (FreeListElement*)f;

    printf("FreeList Address: %p \n", f);
    printf("- Lower Order: %d \n", l);
    printf("- Upper Order: %d \n", u);

    // for (int i = 0; i < u-l+1; i++){
    //     printf("Head addr at order %d : %p\n", i+l, freelist[i].head);
    //     printf("BBM addr at order %d : %p\n", i+l, freelist[i].freebm);
    // }
    // printf("BBM at order %d : %s\n", 11, *(char**)freelist[11].freebm);
    
}
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
};

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

extern void split_block(FreeList f, int current_order, int target_order){
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
    FreeListElement * freelist = mmalloc(sizeof(FreeListElement) * (u-l +1));
    if (freelist == NULL) return NULL;

    for (int i = 0; i < (u-l +1); i++) {
        freelist[i].head = NULL;
        freelist[i].freebm = freelist[i].freebm = bbmcreate(size, l + i);
    }

    return (FreeList) freelist;
} //TODO add error handling for creation of freelist

extern void freelist_set_head(FreeList f, int idx, void *head){
    if (f == NULL) return;
    ((FreeListElement *)f)[idx].head = head;
}

extern void freelist_set_bm(FreeList f, int idx, char * freebm){
    if (f == NULL) return;
    ((FreeListElement *)f)[idx].freebm = freebm;
}

//freeing freelist for allocator 
extern void freelistdelete(FreeList f, int l, int u){
    if (f == NULL) return;
    int total_orders = (u - l) + 1;
    mmfree(f, sizeof(FreeListElement) * total_orders);
}

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u){ //e target order //u is the highest order to take from 
    FreeListElement* free_lists = (FreeListElement*)f;
    void * freeblock; //the block to be allocated

    int target_order = e-l;
    int current_order = e-l;

    if (free_lists[current_order].head == NULL){ //is an exact-size block free? no, so split buddies
        while(free_lists[current_order].head == NULL){ //find order that has free blocks
            if (current_order > u-l){
                break;
            }
            current_order++;
        }
        split_block(f, current_order, target_order);
    
    }
    
    freeblock = free_lists[target_order].head;

    if (free_lists[target_order].head != NULL && *(void**)(free_lists[target_order].head) != NULL) { //if there is a next pointer set head to what is next
        free_lists[target_order].head = *(void**)(free_lists[target_order].head);
    } else { //if there isn't a next pointer set head to null
        free_lists[target_order].head = NULL;
    }

    bbmset(free_lists[target_order+l].freebm, base, freeblock, target_order+l);

    return freeblock;
}

//freeing an allocation in the allocator by putting it back on the freelist and coalescing
extern void freelistfree(FreeList f, void *base, void *mem, int u, int l){
    FreeListElement * free_list = (FreeListElement *)f;
    int current_order = 0;
    int max_order = u-l;

    //loop through each order to find order of allocation at a bit associated with the buddy
    while (current_order <= max_order){
        if (bbmtst(free_list[current_order].freebm, base, mem, current_order+l) == 1){ //found allocation at order
            break;
        }
        current_order++;
    }
     
    //if we exceeded max order without finiding the allocation. invalid free return
    if (current_order > max_order){
        return;
    }

    bbmclr(free_list[current_order].freebm, base, mem, current_order + l);

    //traverse linked list to get free buddy? if not then they cant be coaelesed and buddy will get pushed into freelist at that order
    void * curr_block = mem;
    
    //coalescence if free buddy exists
    while (current_order < max_order) {
        // Calculate the buddy address using your inverted address utility
        void *buddy = baddrinv(base, curr_block, current_order+l);
        
        // Check if buddy is free. (Assuming your bitmap or list traversal confirms this)
        // We will traverse the free list at current_order to look for the buddy.
        void *prev = NULL;
        void *curr = free_list[current_order].head;
        int buddy_found = 0;

        while (curr != NULL) {
            if (curr == buddy) {
                buddy_found = 1;
                break;
            }
            prev = curr;
            // Assuming FreeListElement node design where next pointer is at the start of the node
            curr = *(void **)curr; 
        }

        // If buddy isn't in the free list, it's allocated. We stop merging here.
        if (!buddy_found) {
            break; 
        }

        // 3. Buddy is free! Unlink it from the current order's free list
        if (prev == NULL) {
            free_list[current_order].head = *(void **)buddy;
        } else {
            *(void **)prev = *(void **)buddy;
        }

        // Update bitmap status if your system tracks bit statuses per block
        // bbmclr(free_list[current_order].freebm, base, buddy, current_order);

        // 4. Merge: Update curr_block to point to the lowest starting address of the pair
        if (buddy < curr_block) {
            curr_block = buddy;
        }

        // Move up to the next power-of-two order
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

    printf("FreeList Address: %p \n", f);
    printf("- Lower Order: %d \n", l);
    printf("- Upper Order: %d \n", u);
}


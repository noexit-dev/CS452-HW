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

extern FreeList freelistcreate(size_t size, int l, int u){ //push buddies
    FreeList * freelist = mmalloc(sizeof(FreeListElement) * (u-l + 1));
    (FreeListElement *)


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

extern void *freelistalloc(FreeList f, void *base, int e, int u){ //e target order //u is the highest order to take from 
    FreeListElement* free_lists = (FreeListElement*)f;
    void * freeblock;

    int current_order = e;

    if (free_lists[current_order].head == NULL){ //is an exact-size block free? no, so split buddies
        while(free_lists[current_order].head == NULL){ //find order that has free blocks
            if (current_order > u){
                break;
            }
            current_order = current_order++;
        }

        //split logic
        while (current_order != e){ //split down one order until target order
            void* buddy1 = pop_free_block(f, current_order);
            if (buddy1 == NULL) return;
    
            int target_order = current_order - 1;
            size_t buddy_size = e2size(target_order);
    
            // 2. Calculate the exact byte address of the second buddy
            void* buddy2 = (void*)(buddy1 + buddy_size);
    
            // 3. Push both into the smaller order's tracking struct.
            // This automatically overwrites their first 8 bytes with the updated tracking links.
            push_free_block(f, target_order, buddy2);
            push_free_block(f, target_order, buddy1);

            current_order--;
        }

    }


    
    if (*(void**)(free_lists[e].head) != NULL) { //if there is a next pointer set head to what is next
        free_lists[e].head == *(void**)(free_lists[e].head);
    }
    //if there isn't a next pointer return to caller free to remove
    freeblock = free_lists[e].head;
    free_lists[e].head = NULL;
    return freeblock;
    
}

void push_free_block(FreeList f, int order, void * new_block){
    FreeListElement* free_lists = (FreeListElement*)f;

    if (new_block == NULL) return;

    *(void**)new_block = free_lists[order].head;
    free_lists[order].head = new_block;
};

void* pop_free_block(FreeList f,  int order) {
    FreeListElement* free_lists = (FreeListElement*)f;

    void* block_to_allocate = free_lists[order].head;
    
    if (block_to_allocate == NULL) {
        return NULL;
    }

    void* next_block = *(void**)block_to_allocate;

    free_lists[order].head = next_block;

    return block_to_allocate;
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


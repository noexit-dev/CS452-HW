#ifndef FREELIST_H
#define FREELIST_H

#include <stdio.h>

typedef void *FreeList;

extern FreeList freelistcreate(size_t size, int l, int u);
extern void freelist_set_head(FreeList f, int e, void *head);

extern void freelistdelete(FreeList f, int l, int u);

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u);
extern void push_free_block(FreeList f, int order, void * new_block);
extern void *pop_free_block(FreeList f, int order);
extern void split_block(FreeList f, int current_order, int target_order, int l);

extern void  freelistfree(FreeList f, void *base, void *mem, int u, int l);

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);
extern void freelistprint(FreeList f, int l, int u);

#endif

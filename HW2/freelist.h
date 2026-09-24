#ifndef FREELIST_H
#define FREELIST_H

#include <stdio.h>


/*
Do store management data in free blocks.  In particular,  for each 
block size, maintain a list of free blocks of that size, with a pointer 
stored at the beginning of each block

The Linux-kernel memory-manager document describes how to use 
a bitmap, for each free list, to quickly determine whether a block’s 
buddy is on that list.  A buddy-pair’s bit records that either buddy, 
or both buddies, are allocated

The Linux documentation assumes that a block’s size, and thus its 
free list, is known.  One way to determine a block’s size is to use a 
bitmap, for each free list, to determine whether either buddy block 
was allocated from that list

*/

typedef void *FreeList;

extern FreeList freelistcreate(size_t size, int l, int u);
extern void freelist_set_head(FreeList f, int e, void *head); //f is freelist ptr / e is which order/idx of freelist /  head* is the ptr you would like to set as the head
extern void freelist_set_bm(FreeList f, int e, char *freebm); //f is freelist ptr / e is which order/idx of freelist /  head* is the ptr you would like to set as the head
extern void freelistdelete(FreeList f, int l, int u);

extern void *freelistalloc(FreeList f, void *base, int e, int l, int u);
extern void push_free_block(FreeList f, int order, void * new_block);
extern void *pop_free_block(FreeList f, int order);
extern void split_block(FreeList f, int current_order, int target_order);

extern void  freelistfree(FreeList f, void *base, void *mem, int u, int l);

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);
extern void freelistprint(FreeList f, int l, int u);

#endif

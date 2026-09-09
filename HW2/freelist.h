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
extern void     freelistdelete(FreeList f, int l, int u);

extern void *freelistalloc(FreeList f, void *base, int e, int l);
extern void  freelistfree(FreeList f, void *base, void *mem, int e, int l);

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u);
extern void freelistprint(FreeList f, int l, int u);

#endif

#include <math.h>
#include "utils.h"
#include <sys/mman.h>

//mmalloc calls mmap which is a memory map and it's a function pointer so i believe it will have the location of the mmap as well
extern void * mmalloc(size_t size){
    return mmap(0,size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0); //This may return MAP_FAILED
}

//frees memory map
extern void mmfree(void *p, size_t size){
    if (p == 0) return;
    munmap(p, size);
}

//divides size_t n by size_t d
extern size_t divup(size_t n, size_t d){
    return ceil(1.0 * n / d);
}

//converts bits to bytes
extern size_t bits2bytes(size_t bits){
    return (size_t)(bits / bitsperbyte);
}

extern size_t e2size(int e){
    return (size_t) (pow(2,e));
}

extern int size2e(size_t size){
    return (int) ceil(log2(size));
}

extern unsigned int higher_power_of_2(unsigned int n) {
    if (n == 0) return 1; // 1 is 2^0, the smallest power of 2
    n--;
    
    // Flood all lower bits with 1s
    n |= (n >> 1);
    n |= (n >> 2);
    n |= (n >> 4);
    n |= (n >> 8);
    n |= (n >> 16);
    
    // n is now (higher_power - 1). Adding 1 rolls it over to the next power.
    return n + 1;
}


extern void bitset(void *p, int bit){
    unsigned int mask = 1<<bit;
    *(unsigned int *)p |= mask; 
}

extern void bitclr(void *p, int bit){
    unsigned int mask = ~(1<<bit);
    *(unsigned int *)p &= mask; 
}

extern void bitinv(void *p, int bit){ 
    unsigned int mask = 1<<bit;
    *(unsigned int *)p ^= mask;
}

extern int  bittst(void *p, int bit){
    unsigned int mask = 1<<bit;
    return (*(unsigned int *)p &= mask);
}
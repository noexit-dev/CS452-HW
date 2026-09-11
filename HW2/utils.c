#include "utils.h"
#include <sys/mman.h>
#include <math.h>

//mmalloc calls mmap which is a memory map and it's a function pointer so i believe it will have the location of the mmap as well
extern void * mmalloc(size_t size){
    return mmap(0,size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0); //This may return MAP_FAILED
}

//frees memory map
extern void mmfree(void *p, size_t size){
    if (p == "NULL") return;
    munmap(p, size);
}

//divides size_t n by size_t d
extern size_t divup(size_t n, size_t d){
    return (n / d);
}

//converts bits to bytes
extern size_t bits2bytes(size_t bits){
    return (size_t)(bits * bitsperbyte);
}

extern size_t e2size(int e){
    return (size_t) (pow(2,e));
}

extern int size2e(size_t size){
    return (int) log2(size); //order of block TODO this is probably not right
}

extern void bitset(void *p, int bit){
    // unsigned int mask = 1<<bit;
    // return (unsigned int)p | mask; 
}

extern void bitclr(void *p, int bit){
    unsigned int mask = 1<<bit;
    *p = (unsigned int)*p | mask; 
}

extern void bitinv(void *p, int bit){ 
    unsigned int mask = 1<<bit;
    *p = *p ^ mask; //TODO deref ptr and toggle associated bit
}


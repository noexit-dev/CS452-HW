#include <math.h>
#include "utils.h"
#include <sys/mman.h>

/**
 * @brief returns a page of memory to be allocated from using mmap
 * 
 * @param size - size of allocation
 * @return void* - pointer to start of allocation
 */
extern void * mmalloc(size_t size){
    return mmap(0,size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0); //This may return MAP_FAILED
}

/**
 * @brief frees a page of memory using a pointer using munmap
 * 
 * @param p - pointer to start of allocation
 * @param size - size of allocation to be freed
 */
extern void mmfree(void *p, size_t size){
    if (p == 0) return;
    munmap(p, size);
}

/**
 * @brief divides size_t n by size_t d as a double and takes ceiling of it
 * 
 * @param n - numerator
 * @param d - denominator
 * @return size_t - result of division
 */
extern size_t divup(size_t n, size_t d){
    return ceil(1.0 * n / d);
}

/**
 * @brief converts bits to bytes
 * 
 * @param bits - size bits
 * @return size_t - size bits converted to bytes
 */
extern size_t bits2bytes(size_t bits){
    return (size_t)(bits / bitsperbyte);
}

/**
 * @brief converts order e to size bytes
 * 
 * @param e - order e
 * @return size_t - order e in size bytes
 */
extern size_t e2size(int e){
    return (size_t) (pow(2,e));
}

/**
 * @brief converts size bytes to order e
 * 
 * @param size - size bytes
 * @return int - size bytes in order e
 */
extern int size2e(size_t size){
    return (int) ceil(log2(size));
}

/**
 * @brief function to round size request to nearest power of 2
 * 
 * @param n - unsigned integer to be rounded up
 * @return unsigned int - size rounded to nearest power of 2
 */
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

/**
 * @brief sets specified bit to 1
 * 
 * @param p - pointer to bitmap
 * @param bit - index to bitmap
 */
extern void bitset(void *p, int bit){
    unsigned int mask = 1<<bit;
    *(unsigned int *)p |= mask; 
}

/**
 * @brief clears specified bit to 0
 * 
 * @param p - pointer to bitmap
 * @param bit - index to bitmap
 */
extern void bitclr(void *p, int bit){
    unsigned int mask = ~(1<<bit);
    *(unsigned int *)p &= mask; 
}

/**
 * @brief toggles specified bit
 * 
 * @param p - pointer to bitmap
 * @param bit - index to bitmap
 */
extern void bitinv(void *p, int bit){ 
    unsigned int mask = 1<<bit;
    *(unsigned int *)p ^= mask;
}

/**
 * @brief tests/returns specified bit
 * 
 * @param p - pointer to bitmap
 * @param bit - index to bitmap
 * @return int - specified bit value
 */
extern int bittst(void *p, int bit){
    unsigned int mask = 1<<bit;
    return (*(unsigned int *)p &= mask);
}
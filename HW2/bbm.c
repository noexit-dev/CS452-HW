#include "bbm.h"
#include "bm.h"
#include "utils.h"

/**
 * @brief calculates size of buddy bitmap
 * 
 * @param size - size of pool
 * @param e - order
 * @return size_t - returns size of buddy bitmap
 */
static size_t mapsize(size_t size, int e) {
  size_t blocksize=e2size(e); //calculates size of block using order of block
  size_t blocks=divup(size,blocksize); //
  size_t buddies=divup(blocks,2); 
  return buddies;
}

/**
 * @brief calculates bit index based of offset of mem from base
 * 
 * @param base - base address
 * @param mem - memory address of associated buddy pair
 * @param e - order
 * @return size_t 
 */
static size_t bitaddr(void *base, void *mem, int e) {
  size_t addr=baddrclr(base,mem,e)-base;
  size_t blocksize=e2size(e);
  return addr/blocksize/2;
}

/**
 * @brief creates buddy bitmap
 * 
 * @param size - size of pool
 * @param e - order
 * @return BBM - pointer to buddy bitmap
 */
extern BBM bbmcreate(size_t size, int e) {
  return bmcreate(mapsize(size,e));
}

/**
 * @brief deletes/frees buddy bitmap
 * 
 * @param b - pointer to buddy bitmap
 */
extern void bbmdelete(BBM b) {
  bmdelete(b);
}

/**
 * @brief set associated buddy bit to one (which represents one or both buddies are allocated)
 * 
 * @param b - pointer to buddy bitmap
 * @param base - base address to memory pool
 * @param mem - memory address of allocation
 * @param e - order of allocation
 */
extern void bbmset(BBM b, void *base, void *mem, int e) {
  bmset(b,bitaddr(base,mem,e));
}

/**
 * @brief clear associated buddy bit to 0 (which represents both buddies are free)
 * 
 * @param b - pointer to buddy bitmap
 * @param base - base address to memory pool
 * @param mem - memory address of allocation
 * @param e - order
 */
extern void bbmclr(BBM b, void *base, void *mem, int e) {
  bmclr(b,bitaddr(base,mem,e));
}

/**
 * @brief tests/returns assoicated buddy bit
 * 
 * @param b - pointer to buddy bitmap
 * @param base - base address to memory pool
 * @param mem - memory address of allocation
 * @param e - order
 * @return int - returns associated bit as an integer or 0/NULL if not found or out of bounds
 */
extern int bbmtst(BBM b, void *base, void *mem, int e) {
  return bmtst(b,bitaddr(base,mem,e));
}

/**
 * @brief prints buddy bitmap
 * 
 * @param b - pointer to buddy bitmap
 */
extern void bbmprt(BBM b) { bmprt(b); }

/**
 * @brief sets address to 1
 * 
 * @param base - base address of memory pool
 * @param mem - memory address of allocation
 * @param e - order
 * @return void* - pointer to set address
 */
extern void *baddrset(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return base+((mem-base)|mask);
}

/**
 * @brief clear address to 0
 * 
 * @param base - base address of memory pool
 * @param mem - memory address of allocation
 * @param e - order
 * @return void* - pointer to clear address
 */
extern void *baddrclr(void *base, void *mem, int e) {
  unsigned int mask=~(1<<e);
  return base+((mem-base)&mask);
}

/**
 * @brief toggles address
 * 
 * @param base - base address of memory pool
 * @param mem - memory address of allocation
 * @param e - order
 * @return void* - pointer to toggled address
 */
extern void *baddrinv(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return base+((mem-base)^mask);
}

/**
 * @brief tests address
 * 
 * @param base - base address
 * @param mem - memory address of allocation
 * @param e - order
 * @return int - returns toggled address as an integer
 */
extern int baddrtst(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return (mem-base)&mask;
}
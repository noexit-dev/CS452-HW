#include <stdlib.h>
#include <string.h>

#include "bm.h"
#include "utils.h"

/**
 * @brief finds number of bits in bitmaps
 * 
 * @param b - pointer to the bitmap
 * @return size_t - size of bitmap in bits
 */
static size_t bmbits(BM b) { size_t *bits=b; return *--bits; }

/**
 * @brief finds number of bytes in bitmaps
 * 
 * @param b - pointer to the bitmap
 * @return size_t - size of bitmap in bytes
 */
static size_t bmbytes(BM b) { return bits2bytes(bmbits(b)); }

/**
 * @brief checks if index is within bounds of bitmap
 * 
 * @param b - pointer to the bitmap
 * @param i - index to check against bitmap
 */
static void ok(BM b, size_t i) {
  if (i<bmbits(b))
    return;
  fprintf(stderr,"bitmap index out of range\n");
  exit(1);
}         

/**
 * @brief create a bitmap
 * 
 * @param bits - create a bitmap of size bits
 * @return BM - pointer to the bitmap or 0 if allocation failed
 */
extern BM bmcreate(size_t bits) {
  size_t bytes=bits2bytes(bits);
  size_t *p=mmalloc(sizeof(size_t)+bytes);
  if ((long)p==-1)
    return 0;
  *p=bits;
  BM b=++p;
  memset(b,0,bytes);
  return b;
}

/**
 * @brief deletes bitmap
 * 
 * @param b - pointer to the bitmap to be deleted/freed
 */
extern void bmdelete(BM b) {
  size_t *p=b;
  p--;
  mmfree(p,sizeof(size_t)+bits2bytes(*p));
}

/**
 * @brief sets a specified bit in the bitmap to 1 no matter the state
 * 
 * @param b - pointer to the bitmap
 * @param i - index on the bitmap to be set
 */
extern void bmset(BM b, size_t i) {
  ok(b,i); bitset(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief clears a specified bit in the bitmap to 0 no matter the state
 * 
 * @param b - pointer to the bitmap
 * @param i - index on the bitmap to be cleared
 */
extern void bmclr(BM b, size_t i) {
  ok(b,i); bitclr(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief tests/returns the state of a specified bit on the bitmap
 * 
 * @param b - pointer to the bitmap
 * @param i - index on the bitmap to be tested
 * @return int - bit at index of bitmap represented as an integer
 */
extern int bmtst(BM b, size_t i) {
  ok(b,i); return bittst(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief prints the bitmap
 * 
 * @param b - pointer to the bitmap
 */
extern void bmprt(BM b) {
  for (int byte=bmbytes(b)-1; byte>=0; byte--)
    printf("%02x%s",((char *)b)[byte],(byte ? " " : "\n"));
}
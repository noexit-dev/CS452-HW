#include <stdlib.h>
#include <string.h>

#include "bm.h"
#include "utils.h"

static size_t bmbits(BM b) { size_t *bits=b; return *--bits; }

static size_t bmbytes(BM b) { return bits2bytes(bmbits(b)); }

//checks if given index is within range of bitmap
static void ok(BM b, size_t i) {
  if (i<bmbits(b))
    return;
  fprintf(stderr,"bitmap index out of range\n");
  exit(1);
}         

//allocates memory based on amount of bits requested (including the size of bm)
extern BM bmcreate(size_t bits) {
  size_t bytes=bits2bytes(bits);
  size_t *p=mmalloc(sizeof(size_t)+bytes);
  if ((long)p==-1)
    return 0;
  *p=bits;
  BM b=++p; //increments by size_t to point to the first accessible mem address
  memset(b,0,bytes); //zeros out bytes before use
  return b;
}

extern void bmdelete(BM b) {
  size_t *p=b;
  p--;
  mmfree(p,sizeof(size_t)+bits2bytes(*p));
}

extern void bmset(BM b, size_t i) {
  ok(b,i); bitset(b+i/bitsperbyte,i%bitsperbyte);
}

extern void bmclr(BM b, size_t i) {
  ok(b,i); bitclr(b+i/bitsperbyte,i%bitsperbyte);
}

extern int bmtst(BM b, size_t i) {
  ok(b,i); return bittst(b+i/bitsperbyte,i%bitsperbyte);
}

extern void bmprt(BM b) {
  for (int byte=bmbytes(b)-1; byte>=0; byte--)
    printf("%02x%s",((char *)b)[byte],(byte ? " " : "\n"));
}

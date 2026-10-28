#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "deq.h"

#include "lawn.h"
#include "mole.h"
 
// Modify the program to use your queue, from the earlier homework assign-
// ment. You should only need to change main.c. Functions produce and
// consume should only communicate through a queue

/**
 * @brief produce a mole on the board
 * 
 * @param l - lawn to produce a mole on
 * @param deq - deq tracking moles
 * @return void* - 0 if passing, 1 if failure
 */
static void * produce(Lawn l, Deq deq) { 
  deq_tail_put(deq, mole_new(l,0,0));
  return deq; 
}

static void * consume(Deq deq) { 
  Mole m = deq_head_get(deq);
  if (m != NULL){ //found
      mole_whack(m); 
  }
  
  return 0;
}

int main() {
  srandom(time(0));
  const int n=10;
  Deq deq = deq_new();
  Lawn lawn=lawn_new(0,0);
  for (int i=1; i<=n; i++)
    consume(produce(lawn, deq));
  lawn_free(lawn);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "lawn.h"
#include "mole.h"
#include "thread.h"
#include "deq.h"
#include "mtq.h"
 
// Modify the program to use your queue, from the earlier homework assign-
// ment. You should only need to change main.c. Functions produce and
// consume should only communicate through a queue

/**
 * @brief produce a mole on the board
 * 
 * @param l - lawn to produce a mole on
 * @param mtq - mtq tracking moles
 * @return void* - 0 if passing, 1 if failure
 */
static void * produce(void * a) { 
  void **arg=a;
  Mtq q=(Mtq)arg[0];
  Lawn l=(Lawn)arg[1];
  mtq_tail_put(q, mole_new(l,0,0));
  return 0; 
}

static void * consume(void * a) { 
  void **arg=a;
  Mtq q=(Mtq)arg[0];

  Mole m = mtq_head_get(q);
  if (m != NULL){ //found
      mole_whack(m); 
  }
  
  return 0;
}

int main() {
  srandom(time(0));
  const int n=10;
  Mtq mtq = mtq_new();
  Lawn lawn = lawn_new(0,0);

  void *args[2];

  args[0] = (void *)mtq;
  args[1] = (void *)lawn;

  pthread_t * threads_p = create_thread(produce, n, args);
  pthread_t * threads_c = create_thread(consume, n, args);
  
  wait_thread(threads_p, n);
  wait_thread(threads_c, n);
  
  lawn_free(lawn);
  mtq_del(mtq, 0);
}

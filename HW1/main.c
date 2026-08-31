#include <stdio.h>
#include <stdlib.h>

#include "deq.h"

int main() {
  Deq q=deq_new();

  // ad hoc testing

  // put
  deq_head_put(q, (Data)"1");
  deq_head_put(q, (Data)"2");
  deq_head_put(q, (Data)"3");

  deq_tail_put(q, (Data)"2");
  deq_tail_put(q, (Data)"3");


  //ith
  Data ith_h = deq_head_ith(q, 2);
  Data ith_t = deq_tail_ith(q, 0);

  //get

  // Data get_h = deq_head_get(q);
  // Data get_h2 = deq_head_get(q);
  // Data get_h3 = deq_head_get(q);
  // Data get_h4 = deq_head_get(q);
  // Data get_t = deq_tail_get(q);
  // Data get_t2 = deq_tail_get(q);


  //rem

  Data rem_h = deq_head_rem(q, "1");

  printf("ith_h: %s ith_t: %s \n \n", (char*)ith_h, (char*)ith_t);
  // printf("get_h: %s get_t: %s get_t2: %s \n \n", (char*)get_h, (char*)get_t, (char*)get_t2);
  printf("rem_h: %s \n \n", (char*)rem_h);


  char *s=deq_str(q,0);
  printf("%s\n",s);
  free(s);

  deq_del(q,0);
  return 0;
}

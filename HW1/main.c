#include <stdio.h>
#include <stdlib.h>

#include "deq.h"

/**
 * @brief Creates sample input data for testing deq in main.
 * 
 * The data will be structured like the following:
 * 
 * "4" "2" "1" "2" "3"
 * 
 * This also tests the put method by using both head and tail put.
 * 
 * @param q The deq being used
 */
void createData(Deq q){
    deq_head_put(q, (Data)"1");
    deq_head_put(q, (Data)"2");
    deq_head_put(q, (Data)"4");
  
    deq_tail_put(q, (Data)"2");
    deq_tail_put(q, (Data)"3");
}

/**
 * @brief Prints deq structure without a function mapped to them.
 * 
 * Prints along with specified title.
 * 
 * @param q The deq being used
 */
void printDeq(char * title, Deq q){
  char *s=deq_str(q,0);
  printf("%s %s\n",title, s);
  free(s);
}

int main() {
  Deq q=deq_new();

  //put testing
  createData(q);
  printDeq("Put Test:" ,q);

  //get testing removes 6 items, which is more than the 5 items in the list, which tests empty list
  deq_head_get(q);
  printDeq("Get Head Test 1:" ,q);
  deq_head_get(q);
  printDeq("Get Head Test 2:" ,q);
  deq_head_get(q);
  printDeq("Get Head Test 3:" ,q);

  Data get_t = deq_tail_get(q);
  printDeq("Get Tail Test 1:" ,q);
  Data get_t2 = deq_tail_get(q);
  printDeq("Get Tail Test 2:" ,q);
  Data get_t3 = deq_tail_get(q);
  printDeq("Get Tail Test 3:" ,q);

  printf("get_t: %s get_t2: %s get_t3: %s \n \n", (char*)get_t, (char*)get_t2, (char*)get_t3); //prints last 3 gets to check that the last one returns NULL

  //ith testing with empty list (Should return NULL)
  Data ith_h = deq_head_ith(q, 2);
  printDeq("Ith Head Test 1:" ,q);

  Data ith_t = deq_tail_ith(q, 9);
  printDeq("Ith Tail Test 1:" ,q);
  printf("ith_h: %s ith_t: %s \n \n", (char*)ith_h, (char*)ith_t);
  
  
  //rem testing with empty list (Should return NULL)
  Data rem_h = deq_head_rem(q, "1");
  printDeq("Rem Head Test 1:" ,q);
  Data rem_t = deq_tail_rem(q, "2");
  printDeq("Rem Tail Test 1:" ,q);
  printf("rem_h: %s rem_t: %s \n \n", (char*)rem_h, (char*)rem_t);
  
  createData(q);
  
  //ith testing with populated list and out of bounds
  Data ith_h2 = deq_head_ith(q, 2);
  printDeq("Ith Head Test 2:" ,q);
  Data ith_h3 = deq_head_ith(q, 0);
  printDeq("Ith Head Test 3:" ,q);
  Data ith_t2 = deq_tail_ith(q, 6);
  printDeq("Ith Tail Test 2:" ,q);
  printf("ith_h2: %s ith_h3: %s ith_t2: %s \n \n", (char*)ith_h2, (char*)ith_h3, (char*)ith_t2);

  //rem testing with populated list with endpoint case and not found
  Data rem_h2 = deq_head_rem(q, "4");
  printDeq("Rem Head Test 2:" ,q);
  Data rem_t2 = deq_tail_rem(q, "6");
  printDeq("Rem Tail Test 2:" ,q);
  printf("rem_h2: %s rem_t2: %s \n \n", (char*)rem_h2, (char*)rem_t2);

  deq_del(q,0);
  return 0;
}
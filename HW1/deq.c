#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

//deq stands for doubley-ended doubly-linked queue

// indices and size of array of node pointers
typedef enum {Head,Tail,Ends} End;

typedef struct Node { //Node "object"
  
  struct Node *np[Ends];        // next/prev neighbors

  // creates a pointer array of type Node and makes it of size Ends (3?)
  // probably so index 0 and 1 can have next/prev nodes

  Data data;  //  data of node (could be anything in this implementation)
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len; 
} *Rep;  

static Rep rep(Deq q) { // checks for zero/null pointer, empty list? and return Deq casted as Rep
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

// put: append onto an end (could be either head or tail), len++
static void put(Rep r, End e, Data d) {

  Node newNode = malloc(sizeof(Node)); //create newnode

  



  //zero checking already done by rep
  // rep is deq
  // take data and make node?


  // //need to malloc np pointer array and data

  // newNode->np = malloc(Ends * sizeof(struct Node));

  // // Node *headtail = r->ht[e]; //grabs head or tail node pointer and puts in temp pointer

  // // newnode-> //add as new np pointer without losing list ugh TODO


  // newNode->data = d; //pass in data
  



}

// ith: return by 0-base index, len unchanged
static Data ith(Rep r, End e, int i) {
  if (e >= 2) ERROR("Error: invalid end passed"); // error checking for 
  
  Node currNode = r->ht[e];
  for (int count = 0; count < i; count++){
    currNode = r->ht[e]->np[e];
  } 

  return currNode->data;
}

// get: return from an end (could be either head or tail), len--
static Data get(Rep r, End e) {
  if (e >= 2) ERROR("Error: invalid end passed"); // error checking for 

  Node foundNode = r->ht[e];
  
  //making new head/tail

  r->ht[e] = r->ht[e]->np[e];

  r->len -= 1;

  return foundNode;
}

// rem: return by == comparing, len-- (iff found)
static Data rem(Rep r, End e, Data d) {
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for 
  
  if (r->len == 0) {
    return NULL;
  }

  End opposite = e == Head ? Tail : Head;

  Node currNode = r->ht[e];
  for (int i = 0; i < r->len; i++){
    if (currNode->data == d){
      Node prev = currNode->np[opposite];
      Node next = currNode->np[e];
      prev->np[e] = next;
      break;
    }
    currNode = currNode->np[e];
  }
  if (currNode == NULL) {
    return NULL;
  }

  return currNode->data;

}

extern Deq deq_new() {
  Rep r=(Rep)malloc(sizeof(*r));
  if (!r) ERROR("malloc() failed");
  r->ht[Head]=0;
  r->ht[Tail]=0;
  r->len=0;
  return r;
}

extern int deq_len(Deq q) { return rep(q)->len; }

extern void deq_head_put(Deq q, Data d) {        put(rep(q),Head,d); }
extern Data deq_head_get(Deq q)         { return get(rep(q),Head);   }
extern Data deq_head_ith(Deq q, int i)  { return ith(rep(q),Head,i); }
extern Data deq_head_rem(Deq q, Data d) { return rem(rep(q),Head,d); }

extern void deq_tail_put(Deq q, Data d) {        put(rep(q),Tail,d); }
extern Data deq_tail_get(Deq q)         { return get(rep(q),Tail);   }
extern Data deq_tail_ith(Deq q, int i)  { return ith(rep(q),Tail,i); }
extern Data deq_tail_rem(Deq q, Data d) { return rem(rep(q),Tail,d); }

extern void deq_map(Deq q, DeqMapF f) {
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail])
    f(n->data);
}

extern void deq_del(Deq q, DeqMapF f) { //free function is so that we know what to free inside of it
  if (f) deq_map(q,f);
  Node curr=rep(q)->ht[Head];
  while (curr) {
    Node next=curr->np[Tail];
    free(curr);
    curr=next;
  }
  free(q);
}

extern Str deq_str(Deq q, DeqStrF f) {
  char *s=strdup("");
  for (Node n=rep(q)->ht[Head]; n; n=n->np[Tail]) {
    char *d=f ? f(n->data) : n->data;
    char *t; asprintf(&t,"%s%s%s",s,(*s ? " " : ""),d);
    free(s); s=t;
    if (f) free(d);
  }
  return s;
}

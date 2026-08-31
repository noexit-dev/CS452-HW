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
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for if it is an invalid end

  End inverse = e == Head ? Tail : Head; //grab inverse based on what end is given

  Node newNode = malloc(sizeof(*newNode)); //create newnode
  if (newNode == NULL) return;

  newNode->np[e] = 0;
  newNode->np[inverse] = 0;
  newNode->data = d;

  Node currNode = r->ht[e]; //grab ht node

  if (r->len == 0) { //empty list
    r->ht[e] = newNode;
    r->ht[inverse] = newNode;

  } else { //general case
  
    newNode->np[inverse] = currNode; //sets prev node for newnode to ht
  
    currNode->np[e] = newNode; //sets next node for ht to newnode
  
    r->ht[e] = newNode; //sets new ht to newnode
  }

  r->len += 1; //increment len
}

// ith: return by 0-base index, len unchanged
static Data ith(Rep r, End e, int i) {
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for if it is an invalid end
  
  if (r->len == 0) { //if list is of len 0
    return NULL;
  }

  //TODO: add bound checking (return 0 if out of bounds)

  End inverse = e == Head ? Tail : Head; //grab inverse based on what end is given

  Node currNode = r->ht[e]; // grabs ht based on end given
  for (int count = 0; count < i; count++){ // increment until i index
    currNode = currNode->np[inverse]; //grab next node 
  } 

  return currNode->data; //return data stored in node
}

// get: return from an end (could be either head or tail), len--
static Data get(Rep r, End e) {
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for if it is an invalid end

  if (r->len == 0) { //if list is of len 0
    return NULL;
  }

  End inverse = e == Head ? Tail : Head; //grab inverse based on what end is given

  Node currNode = r->ht[e]; //get currNode starting at given end

  Node newEnd = currNode->np[inverse]; //grab next pointer to be the new end
  r->ht[e] = newEnd; //set ht to next pointer

  if (newEnd){ //if newEnd exists set pointer to old ht to null
    r->ht[e]->np[e] = NULL;
  } else { //if not then set inverse to null and stay empty
    r->ht[inverse] = NULL; //list is empty
  }

  r->len -= 1; //decrement len

  Data dataCurr = currNode->data; 

  free(currNode);
  return dataCurr; //if found return data
}

// rem: return by == comparing, len-- (iff found)
static Data rem(Rep r, End e, Data d) {
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for if it is an invalid end

  if (r->len == 0) { //if list is of len 0
    return NULL;
  }

  End inverse = e == Head ? Tail : Head; //grab inverse based on what end is given

  Node currNode = r->ht[e]; //get currNode starting at given end

  if (r->len != 1){ //len is not 1

    for (int i = 0; i < r->len; i++){ //increment through list starting at given end until node is found
      
      if (currNode->data == d){
        Node next = currNode->np[inverse]; //grab next node

        if (currNode == r->ht[Head] || currNode == r->ht[Tail]){ //update ht node to be the next node if ht removed
          r->ht[e] = next;
        }

        Node prev = currNode->np[e]; //grab prev node
        prev->np[inverse] = next; //set prev next pointer to next to prepare for removal
        next->np[e] = prev; //set next prev pointer to prev to prep for removal
        break;
      }
      currNode = currNode->np[e]; //go to next node
    }
    if (currNode == NULL) { //if currNode is null, then not found and return null
      return NULL;
    }
  } else { 
    if (currNode != d) { // len is 1 and is not == to d
      return NULL;
    } else {
      r->ht[Head] = NULL;
      r->ht[Tail] = NULL;
    }
  }
    
  r->len -= 1; //decrement len

  Data dataCurr = currNode->data; 

  free(currNode);
  return dataCurr; //if found return data
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "deq.h"
#include "error.h"

typedef enum {Head,Tail,Ends} End;

typedef struct Node {
  struct Node *np[Ends];        // next/prev neighbors
  Data data;
} *Node;

typedef struct {
  Node ht[Ends];                // head/tail nodes
  int len; 
} *Rep;  

static Rep rep(Deq q) {
  if (!q) ERROR("zero pointer");
  return (Rep)q;
}

/**
 * @brief appends data as a node onto an end and increment length
 * 
 * Dynamically allocates space for a new node and copies data to it. Appends
 * new node to the end specified (ex. Head or Tail). Increments list length
 * when operation is successful.
 * 
 * @param r Pointer to the list desired.
 * @param e End to append on.
 * @param d Data to store.
 */
static void put(Rep r, End e, Data d) {
  if (e >= Ends) ERROR("Error: invalid end passed");
  
  Node newNode = malloc(sizeof(*newNode));
  if (!newNode) ERROR("malloc() failed");
  
  End inverse = e == Head ? Tail : Head;
  
  newNode->np[e] = NULL;
  newNode->np[inverse] = NULL;
  newNode->data = d;

  Node currNode = r->ht[e];

  if (r->len == 0) { //empty list
    r->ht[e] = newNode;
    r->ht[inverse] = newNode;

  } else { //general case

    newNode->np[inverse] = currNode; //sets next node for newnode to currnode
    currNode->np[e] = newNode; //sets prev node for currnode to newnode
    r->ht[e] = newNode; //sets new ht to newnode
  }

  r->len++;
}

/**
 * @brief Finds the node at specified index and returns the data
 * 
 * Searches list starting at specified end until it finds the node at the matching
 * index. Returns data of node when operation is successful.
 * 
 * @param r Pointer to the list desired.
 * @param e End to start search.
 * @param i Index to search.
 * 
 * @return The data contained inside the node found.
 * @retval NULL returned if length of list is 0 or index is out of bounds.
 * 
 * @note The caller will take ownership if NULL gets returned and will handle error
 * printing if neccessary.
 */
static Data ith(Rep r, End e, int i) {
  if (e >= Ends) ERROR("Error: invalid end passed");
  
  if (r->len == 0) return NULL;

  End inverse = e == Head ? Tail : Head;

  Node currNode;
  int count = 0;
  
  for (currNode = r->ht[e]; currNode != NULL && count < i; currNode = currNode->np[inverse]) {
    count++;
  }

  if (currNode == NULL) return NULL; //index out of bounds

  return currNode->data;
}

/**
 * @brief Returns the data from the end specified and remove the node from the list.
 * 
 * Grabs and removes node from specified end in the list. Decrements
 * length and returns the data when operation is successful.
 * 
 * @param r Pointer to the list desired.
 * @param e End to start search.
 * 
 * @return The data contained inside the node from that end.
 * @retval NULL returned if length of list is 0.
 * 
 * @note The caller will take ownership if NULL gets returned and will handle error
 * printing if neccessary.
 */
static Data get(Rep r, End e) {
  if (e >= Ends) ERROR("Error: invalid end passed"); // error checking for if it is an invalid end

  if (r->len == 0) return NULL; //if empty list then return NULL

  End inverse = e == Head ? Tail : Head;

  Node currNode = r->ht[e];

  Node newEnd = currNode->np[inverse]; //grab next pointer to be the new end
  r->ht[e] = newEnd; //set ht to point to new end

  if (newEnd){ //if newEnd exists set pointer to old ht to null
    newEnd->np[e] = NULL;
  } else { //empty list
    r->ht[inverse] = NULL;
  }

  r->len--;
  Data dataCurr = currNode->data; 
  free(currNode);
  return dataCurr;
}

/**
 * @brief Removes node from list when matching data is found.
 * 
 * Starts from specified end and compares the data of each node until found
 * or hits the end of the list. Decrements length and returns the data of 
 * removed node when operation is successful.
 * 
 * @param r Pointer to the list desired.
 * @param e End to start search.
 * @param d Data to search.
 * 
 * @return The data contained inside the node removed.
 * @retval NULL returned if length of list is 0 or not found.
 * 
 * @note The caller will take ownership if NULL gets returned and will handle error
 * printing if neccessary.
 */
static Data rem(Rep r, End e, Data d) {
  if (e >= Ends) ERROR("Error: invalid end passed"); 

  if (r->len == 0) return NULL;

  End inverse = e == Head ? Tail : Head;

  Node currNode;
  Data dataCurr = NULL;

  for (currNode = r->ht[e]; currNode != NULL && currNode->data != d; currNode = currNode->np[inverse]) {
  }

  if (currNode == NULL) { //not found
    return dataCurr; 
  } else { //data is found
    Node next = currNode->np[inverse];
    Node prev = currNode->np[e];
    
    if (prev != NULL){
      prev->np[inverse] = next;
    }
    if(next != NULL){
      next->np[e] = prev;
    }

    // endpoint case handling
    if (currNode == r->ht[Head]) {
      r->ht[Head] = next;
    }
    if (currNode == r->ht[Tail]) {
      r->ht[Tail] = prev;
    }
    
    dataCurr = currNode->data;
    free(currNode);
    r->len--;
    return dataCurr;
  }
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

extern void deq_del(Deq q, DeqMapF f) {
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

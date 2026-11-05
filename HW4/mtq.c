#include <pthread.h>

#include "deq.h"
#include "mtq.h"

static pthread_mutex_t lock;
static pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER;

Mtq mtq_new(){
    pthread_mutex_init(&lock,0);
    
    Deq mtq;

    pthread_mutex_lock(&lock);
    mtq = deq_new();
    pthread_mutex_unlock(&lock);

    return (Mtq)mtq;
}

void mtq_del(Mtq q, DeqMapF f){
    pthread_mutex_lock(&lock);
    deq_del((Deq)q,f);
    pthread_mutex_unlock(&lock);
}

void mtq_tail_put(Mtq q, Data d){
    pthread_mutex_lock(&lock);
    deq_tail_put((Deq)q, d);
    pthread_cond_signal(&not_empty);
    pthread_mutex_unlock(&lock);
}

Data mtq_head_get(Mtq q){
    Data data;

    pthread_mutex_lock(&lock);
    // Producers may still be creating moles when consumers arrive.
    while (deq_len((Deq)q) == 0)
        pthread_cond_wait(&not_empty, &lock);
    data = deq_head_get((Deq)q);
    pthread_mutex_unlock(&lock); 
    
    return data;
}

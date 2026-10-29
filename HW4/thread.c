#include <stdlib.h>
#include <pthread.h>
#include "thread.h"

// call consume n times creating n threads
// it will consume n produce calls which creates another n threads

//it doesn't matter the order it will consume n many produces as a queue

pthread_t * create_thread(void * (*func)(void *), int num_of_threads, void * arg){
    
    pthread_t * tids = malloc((size_t)num_of_threads * sizeof *tids);
    if(!tids) return NULL;

    for (int i = 0; i < num_of_threads; i++){
        pthread_create(&tids[i], 0, func, arg);
    }
    return tids;
}

void wait_thread(pthread_t * t, int n){
    for (int i = 0; i < n; i++){
        pthread_join(t[i], 0);
    }
    free(t);
}

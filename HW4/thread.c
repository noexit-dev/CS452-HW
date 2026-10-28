#include <pthread.h>
#include "thread.h"

pthread_t * thread;
int n = 0;

int create_thread(void * func, int num_of_threads, void * arg){
    
    thread = malloc(num_of_threads * sizeof(pthread_t));
    n = num_of_threads;

    for (int i = 0; i < num_of_threads; i++){
        if (arg) //null check
        pthread_create(&thread[i], NULL, func, arg);
    }
    return 0;
}


void wait_thread(void * thread){
    for (int i = 0; i < n; i++){
        pthread_join(&thread[i], NULL);
    }
}
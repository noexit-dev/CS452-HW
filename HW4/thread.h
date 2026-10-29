void ** thread;

pthread_t * create_thread(void * (*func)(void *), int num_of_threads, void * arg);

void wait_thread(pthread_t * t, int n);
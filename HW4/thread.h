

void ** thread;

int create_thread(void * func, int num_of_threads, void * arg);

void wait_thread(void ** thread);
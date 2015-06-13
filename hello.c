
#include <unistd.h>
#include <pthread.h>

const char* msg = "Hello World\n";

#define THREAD_COUNT 4
#define TOTAL_WRITES 100e6

int written = 0;

void hello_worlds() {

    while (__sync_add_and_fetch(&written, 1) < TOTAL_WRITES) {
        write(1, msg , 12);
    }

    pthread_exit(0);

}

int main() {

    pthread_t threads[THREAD_COUNT];

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_t thread;
        threads[i] = thread;
        pthread_create(&threads[i], NULL, (void *) &hello_worlds, NULL);
    }

    for (int i = 0; i < THREAD_COUNT; ++i) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}



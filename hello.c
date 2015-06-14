
// required for cpu affinity
// #define _GNU_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

const char* msg = "Hello World\n";

int written = 0;
int total_writes = 0;
int thread_ct = 0;

#define THREAD_STACK_SIZE 1<<10

void hello_worlds() {

    int n = 0;

    while (n < total_writes) {
        write((n&1)+1, msg , 12);
        n = __sync_add_and_fetch(&written, 1);
    }

    pthread_exit(0);

}

int main(int argc, char* argv[]) {

    if (argc < 3) {
        printf("Please specify a threadct and writect\n");
        return 1;
    }

    int cpu_ct = sysconf(_SC_NPROCESSORS_ONLN);

    total_writes = atoi(argv[1]);
    thread_ct = atoi(argv[2]);

    printf("Detected %d cpus, running %d threads, write ct is %d\n", cpu_ct, thread_ct, total_writes);

    pthread_t* threads = malloc(sizeof(pthread_t) * thread_ct);

    for (int i = 0; i < thread_ct; ++i) {
        // set stack size
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        void* sp;
        posix_memalign(&sp, sysconf(_SC_PAGESIZE), THREAD_STACK_SIZE);
        pthread_attr_setstack(&attr, sp, THREAD_STACK_SIZE);
        // create the thread
        pthread_create(&threads[i], &attr, (void *) &hello_worlds, NULL);
        // set the affinity - non-portable
        // cpu_set_t cpuset;
        // CPU_ZERO(&cpuset);
        // CPU_SET(i, &cpuset);
        // pthread_setaffinity_np(threads[i], sizeof(cpu_set_t), &cpuset);
    }

    for (int i = 0; i < thread_ct; ++i) {
        pthread_join(threads[i], NULL);
    }

    free(threads);

    return 0;

}

#include <stdio.h>
#include "queue.h"
#include <pthread.h>
#include <stdlib.h>

void *send_message(void *data)
{
    multiqueue_t *queue = data;
    pthread_t *tid = malloc(sizeof(pthread_t));
    *tid = pthread_self();
    fprintf(stderr, "Send message from thread with uid = %lu\n", *tid);
    multiqueue_push(queue, tid);
}

void *receive_message(void *data)
{
    multiqueue_t *queue = data;
    fprintf(stderr, "Receive message with thread uid = %lu\n", pthread_self());

    pthread_t *tid;
    multiqueue_pop(queue, (void **)&tid);
    fprintf(stderr, "Received message from thread with uid = %lu\n", *tid);
    free(tid);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s recv_thread_count send_thread_count\n", argv[0]);
        return 1;
    }

    int recv_count = atoi(argv[1]);
    int send_count = atoi(argv[2]);

    multiqueue_t *queue = multiqueue_init();

    pthread_t *receive_threads = malloc(recv_count * sizeof(pthread_t));
    pthread_t *send_threads = malloc(send_count * sizeof(pthread_t));

    for (int i = 0; i < recv_count; i++) {
        if (pthread_create(&receive_threads[i], NULL, receive_message, queue)) {
            break;
        }
    }

    for (int i = 0; i < send_count; i++) {
        if (pthread_create(&send_threads[i], NULL, send_message, queue)) {
            break;
        }
    }

    for (int i = 0; i < recv_count; i++) {
        pthread_join(receive_threads[i], NULL);
    }

    for (int i = 0; i < send_count; i++) {
        pthread_join(send_threads[i], NULL);
    }

    free(send_threads);
    free(receive_threads);
    multiqueue_free(queue);

    return 0;
}

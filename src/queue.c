#include "queue.h"
#include <stdlib.h>
#include <pthread.h>

typedef struct node node_t;

struct node
{
    node_t *next;
    node_t *prev;
    void *data;
};

struct multiqueue {
    node_t *head;
    node_t *tail;
    size_t size;
    pthread_mutex_t mutex;
    pthread_mutex_t pop_mutex;
    pthread_cond_t empty;
};

multiqueue_t *multiqueue_init()
{
    multiqueue_t *queue = calloc(1, sizeof(multiqueue_t));

    if (pthread_mutex_init(&queue->mutex, NULL) ||
        pthread_mutex_init(&queue->pop_mutex, NULL) ||
        pthread_cond_init(&queue->empty, NULL))
    {
        multiqueue_free(queue);
        queue = NULL;
    }

    return queue;
}

void multiqueue_free(multiqueue_t *queue)
{
	if (!queue)
		return;

    while (queue->head){
		node_t *tmp = queue->head;
		queue->head = queue->head->next;
		free(tmp);
	}

    pthread_mutex_destroy(&queue->mutex);
    pthread_mutex_destroy(&queue->pop_mutex);
    pthread_cond_destroy(&queue->empty);
    free(queue);
}

void multiqueue_push(multiqueue_t *queue, void *data)
{
    pthread_mutex_lock(&queue->mutex);

    node_t *new_head = new_head = calloc(1, sizeof(node_t));

	new_head->data = data;
	new_head->next = queue->head;
    queue->head = new_head;

    if (queue->size != 0) {
        queue->head->next->prev = queue->head;
    }
    else {
        queue->tail = queue->head;
        pthread_cond_signal(&queue->empty);
    }
    queue->size++;

    pthread_mutex_unlock(&queue->mutex);
}

void multiqueue_pop(multiqueue_t *queue, void **data) {
    pthread_mutex_lock(&queue->pop_mutex);
    pthread_mutex_lock(&queue->mutex);

    if (queue->size == 0) {
        pthread_cond_wait(&queue->empty, &queue->mutex);
    }

    if (data) {
        *data = queue->tail->data;
    }
    if (queue->size == 1) {
        free(queue->head);
        queue->tail = queue->head = NULL;
    }
    else {
        node_t *tmp = queue->tail;
        queue->tail = queue->tail->prev;
        free(tmp);
        queue->tail->next = NULL;
    }

    queue->size--;

    pthread_mutex_unlock(&queue->mutex);
    pthread_mutex_unlock(&queue->pop_mutex);
}
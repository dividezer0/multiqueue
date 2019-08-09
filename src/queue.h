#ifndef MULTIQUEUE_H
#define MULTIQUEUE_H

typedef struct multiqueue multiqueue_t;

multiqueue_t *multiqueue_init();
void multiqueue_free(multiqueue_t *queue);

void multiqueue_push(multiqueue_t *queue, void *data);
void multiqueue_pop(multiqueue_t *queue, void **data);

#endif

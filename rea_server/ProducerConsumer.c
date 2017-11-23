#include <stdio.h>  
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#include "ProducerConsumer.h"

LocksPtr Locks_create() {
    LocksPtr l = (LocksPtr) malloc(sizeof (Locks));

    pthread_mutex_init(&(l->mtx), 0);
    pthread_cond_init(&(l->cond_nonempty), 0);
    pthread_cond_init(&(l->cond_nonfull), 0);

    return l;
}

void Locks_destroy(LocksPtr l) {
    pthread_cond_destroy(&(l->cond_nonempty));
    pthread_cond_destroy(&(l->cond_nonfull));
    pthread_mutex_destroy(&(l->mtx));
}

ProducerConsumerQueuePtr ProducerConsumerQueue_Create(int maxsize) {
    ProducerConsumerQueuePtr p = (ProducerConsumerQueuePtr) malloc(sizeof (ProducerConsumerQueue));

    p->start = 0;
    p->end = -1;
    p->count = 0;
    p->maxsize = maxsize;
    p->data = (malloc(sizeof (int)*maxsize));

    p->locksptr = Locks_create();
    
    return p;
}

void ProducerConsumerQueue_Destroy(ProducerConsumerQueuePtr p) {
    Locks_destroy(p->locksptr);
    free(p->data);
    free(p);
}

void ProducerConsumerQueue_insert(ProducerConsumerQueuePtr p, int data) {
    pthread_mutex_lock(&(p->locksptr->mtx));
    
    while (p->count == p->maxsize) {
        pthread_cond_wait(&(p->locksptr->cond_nonfull), &(p->locksptr->mtx));                
    }
    
    p->end = (p->end + 1) % p->maxsize;
    p->data[p->end] = data;
    p->count++;

    pthread_cond_broadcast(&(p->locksptr->cond_nonempty));

    pthread_mutex_unlock(&(p->locksptr->mtx));
}

int ProducerConsumerQueue_remove(ProducerConsumerQueuePtr p) {
    int data = 0;

    pthread_mutex_lock(&(p->locksptr->mtx));
    
    while (p->count == 0) {
        pthread_cond_wait(&(p->locksptr->cond_nonempty), &(p->locksptr->mtx));
    }
    
    data = p->data[p->start];
    p->start = (p->start + 1) % p->maxsize;
    p->count--;

    pthread_cond_broadcast(&(p->locksptr->cond_nonfull));

    pthread_mutex_unlock(&(p->locksptr->mtx));

    return data;
}

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */



#ifndef PRODUCERCONSUMER_H
#define PRODUCERCONSUMER_H

typedef struct Locks {
    pthread_mutex_t mtx;
    pthread_cond_t cond_nonempty;
    pthread_cond_t cond_nonfull;
} Locks;

typedef Locks * LocksPtr;

typedef struct ProducerConsumerQueue {
    int * data;
    int maxsize;
    int start;
    int end;
    int count;
    
    LocksPtr locksptr;
} ProducerConsumerQueue;

typedef ProducerConsumerQueue * ProducerConsumerQueuePtr;


LocksPtr Locks_create();
void Locks_destroy(LocksPtr locks);

ProducerConsumerQueuePtr ProducerConsumerQueue_Create(int maxsize);

void ProducerConsumerQueue_Destroy(ProducerConsumerQueuePtr t);

void ProducerConsumerQueue_insert(ProducerConsumerQueuePtr p, int data);

int ProducerConsumerQueue_remove(ProducerConsumerQueuePtr p);

#endif /* PRODUCERCONSUMER_H */


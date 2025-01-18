//
//  Queue.h
//  AsyncPrinter
//
//  Created by Asaf Baibekov on 03/01/2023.
//

#ifndef Queue_h
#define Queue_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "Node.h"


typedef struct Queue {
	Node *head;
	Node *tail;
	size_t size;
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t not_empty;
} Queue;

Queue *queue_init(void);

void queue_enqueue(Queue *queue, char *message, int number);

char *queue_dequeue(Queue *queue);

void queue_print(Queue *queue);

void queue_clear(Queue *queue);

void queue_free(Queue *queue);

#endif /* Queue_h */

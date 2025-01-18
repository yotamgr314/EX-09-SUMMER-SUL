//
//  Queue.c
//  AsyncPrinter
//
//  Created by Asaf Baibekov on 03/01/2023.
//

#include "Queue.h"

typedef struct {
	Queue *queue;
	Node *node;
} Cleanup_Args;

void cleanup_function(void *arg) {
	Cleanup_Args *args = arg;
	printf("thread num %ld and this message number %d was canceled and I died", (long) pthread_self(), args->node->number);
	free(args->node);
	args->node = NULL;
	queue_free(args->queue);
	args->queue = NULL;
}

Queue *queue_init(void) {
	Queue *queue = malloc(sizeof(Queue));
	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;
	queue->count = 0;
	pthread_mutex_init(&queue->mutex, NULL);
	pthread_cond_init(&queue->not_empty, NULL);
	return queue;
}

// Push a new message onto the queue
void queue_enqueue(Queue *queue, char *message, int number) {
	pthread_mutex_lock(&queue->mutex);
	Node *node = initNode(message, number);
	Cleanup_Args args = { queue, node };
	pthread_cleanup_push(cleanup_function, &args);
	if (queue->tail == NULL) { // The queue is empty
		queue->head = node;
		queue->tail = node;
	} else { // Add the new node to the end of the queue
		queue->tail->next = node;
	}
	pthread_cleanup_pop(0);
	queue->size++;
	queue->count++;
	pthread_cond_signal(&queue->not_empty); // signal the thread which is cond_waiting on the not_empty signal that there is a new event task in the shared event_queue waiting to be consumed. 
	pthread_mutex_unlock(&queue->mutex);
}

// Pop the next message off the queue
char *queue_dequeue(Queue *queue) {
	pthread_mutex_lock(&queue->mutex);
	while (queue->head == NULL) { // in case there is no event task in the shared_event_queue wait on a signal that there is a new event.. (that way we ensure request which the server recived are not ignored if he is busy).
		pthread_cond_wait(&queue->not_empty, &queue->mutex);
	}
	char *message = queue->head->message;
	Node *old_head = queue->head;
	Cleanup_Args args = { queue, old_head };
	pthread_cleanup_push(cleanup_function, &args);
	queue->head = queue->head->next;
	if (queue->head == NULL) { // The queue is now empty
		queue->tail = NULL;
	}
	queue->size--;
	pthread_cleanup_pop(0);
	free(old_head);
	pthread_mutex_unlock(&queue->mutex);
	return message;
}

void queue_print(Queue *queue) {
	pthread_mutex_lock(&queue->mutex);
	Node *current = queue->head;
	while (current != NULL) {
		printf("%s\n", current->message);
		current = current->next;
	}
	pthread_mutex_unlock(&queue->mutex);
}

void queue_clear(Queue *queue) {
	pthread_mutex_lock(&queue->mutex);
	Node *curr = queue->head;
	while (curr != NULL) {
		Node *temp = curr;
		curr = curr->next;
		free(temp);
	}
	queue->head = NULL;
	queue->tail = NULL;
	pthread_mutex_unlock(&queue->mutex);
}

void queue_free(Queue *queue) {
	queue_clear(queue);
	free(queue);
}

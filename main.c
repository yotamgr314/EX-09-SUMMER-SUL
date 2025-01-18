#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "Queue.h"


int print(char *message) {
	printf("%s\n", message);
	return 0;
}


// Thread function to enqueue messages.
void *producer_thread_func(void *args) { 
	Queue *queue = args;
	
	for (int i = 0; i < 10; i++) 
	{
		for (int j = 0; j < 3; j++)
		{
			char *message;
			// int asprintf(char **pointer-to-Char*, const char *format, ...); NOTE: asprintf automatically allocates memory to fit the formatted string, including the null terminator (\0).
			asprintf(&message, "hey, I am thread num %ld and this message number %d", (long) pthread_self(), queue->count); // The `asprintf` function handles both the memory allocation and string formatting in a single step.
			queue_enqueue(queue, message, queue->count);
		}
		sleep(1);
	}
	return NULL;
}



void freeMessage(void *message) {
	printf("==============\n");
	printf("the message\n");
	print((char *) message);
	printf("was canceled\n");
	free(message);
}

void *consumer_thread_func(void *args) {
	Queue *queue = args;
	
	while (1) {
		char *message = queue_dequeue(queue);
		pthread_cleanup_push(freeMessage, message)
		print(message);
		pthread_cleanup_pop(0);
		free(message);
	}
	return NULL;
}

int main(int argc, const char * argv[]) {
	
	pthread_t t1, t2, t3;
	pthread_t t4, t5, t6;
	
	Queue *queue = queue_init();
	pthread_create(&t1, NULL, producer_thread_func, queue);
	pthread_create(&t2, NULL, producer_thread_func, queue);
	pthread_create(&t3, NULL, producer_thread_func, queue);

	pthread_create(&t4, NULL, consumer_thread_func, queue);
	pthread_create(&t5, NULL, consumer_thread_func, queue);
	pthread_create(&t6, NULL, consumer_thread_func, queue);

//	pthread_join(t1, NULL);
//	pthread_join(t2, NULL);
//	pthread_join(t3, NULL);
//
//	pthread_join(t4, NULL);
//	pthread_join(t5, NULL);
//	pthread_join(t6, NULL);
	
	sleep(4);
	
	
	pthread_cancel(t1);
	pthread_cancel(t2);
	pthread_cancel(t3);
	pthread_cancel(t4);
	pthread_cancel(t5);
	pthread_cancel(t6);
	
	queue_free(queue);
	return 0;
}



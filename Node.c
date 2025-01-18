//
//  Node.c
//  AsyncPrinter
//
//

#include "Node.h"

Node *initNode(char *message, int number) {
	Node *node = malloc(sizeof(Node));
	node->message = message;
	node->next = NULL;
	return node;
}

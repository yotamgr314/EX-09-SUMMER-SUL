//
//  Node.h
//  AsyncPrinter
//
//  Created by Asaf Baibekov on 03/01/2023.
//

#ifndef Node_h
#define Node_h

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
	char *message;
	int number;
	struct Node *next;
} Node;

Node *initNode(char *message, int number);

#endif /* Node_h */

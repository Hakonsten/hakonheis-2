#pragma once

/*
DEFINITION OF OUR QUEUESYSTEM:
head - is the node that holds the CURRENT ORDER that will be executed by the elevator
head->next - Is the next order etc..
When adding nodes - we add to the end of the queue
When removing nodes - we remove the current head and define the next node as head. head->next = head;

*/


typedef struct node {
	int data;       // <-------- THIS LINE MIGHT NEED EDITING (val) 
	struct node* next;
} node;

node* queue_initialize_linkedList();

void queue_add_node_linkedList(node* head, int val); // <-------- THIS LINE MIGHT NEED EDITING (val) 

void queue_remove_node_linkedList(node* head);

int queue_linkedList_is_empty(node* head);

int queue_get_headNode(node* head);
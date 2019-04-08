#include "queue_system.h"
#include "elev.h"
#include "io.h"
#include "channels.h"
#include <stdio.h>
#include <stdlib.h>





































































//define and create head node
node* queue_initialize_linkedList() {
	node* head = NULL; 
	head = malloc(sizeof(node)); //Allocate first node of linkedlist
	return head;
}


//Add new nodes
void queue_add_node_linkedList(node* head, int val) { // <-------- THIS LINE MIGHT NEED EDITING (val) 
	node* current = head;

	//current node iterates over entire linkedlist
	while (current->next != NULL) {
		current = current->next; 
	}

	current->next = malloc(sizeof(node)); //after its done iterating add value
	current->next->data = val; // <-------- THIS LINE MIGHT NEED EDITING (val) 
	current->next->next = NULL;
}


//remove current head node and set next as head node
void queue_remove_node_linkedList(node* head){

	node *tmp;         //  Make a temporary node.
	tmp = head;        //Point this temporary node to the head of the stack
	head = head->next; //Point the ‘head’ pointer to the node next to the current head node.
	free(tmp);         //Delete the temporary node using the ‘free’ function.
}


//use Queue_InitializeLinkedList() to be taken into this bool function
int queue_linkedList_is_empty(node* head) {
	return head == NULL;
}


//return value of head node
int queue_get_headNode(node* head) {  // 
	return head->data;  //<--------THIS LINE MIGHT NEED EDITING (not sure if return type int? Depend on elevator order definition
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Create a node given the info struct
struct Node* createNode(struct Info* info) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->info = info;
	newNode->children = NULL;
	newNode->next = NULL;
	return newNode;
}

// Create a list
struct NodeList* createList(int tsize){
	struct NodeList* newlist = (struct NodeList*)malloc(sizeof(struct NodeList));
	newlist->size = tsize;
	newlist->head = (struct Node*)malloc(sizeof(struct Node));
	newlist->head->info = NULL;
	newlist->head->children = NULL;
	newlist->head->next = NULL;
	return newlist;
}

// Add target to list
void addTarget(struct NodeList* list, struct Info* info){
	struct Node* curr = list->head;
	while (curr->next != NULL) {
		curr = curr->next;	
	}	
	struct Node* newnode = createNode(info);
	curr->next = newnode;
}

// Finds a node with target name parameter
struct Node* findTarget(struct NodeList* list, char* target) {
	struct Node* curr = list->head;
	while (curr->next != NULL && strcmp(curr->next->info->target, target) != 0) {
		curr = curr->next;
	}
	return curr->next;
}

int containsCycle(struct NodeList* graph) {
	int size = graph->size;
	struct NodeList* visited = createList(size);
	struct NodeList* rec = createList(size);
	struct Node* curr = graph->head->next;
	
	for (int i = 0; i < size; i++) {
		if (isCycle(graph, curr, visited, rec) == 1) {
			return 1;
		}
		curr = curr->next;
	}
	return 0;
}

int isCycle(struct NodeList* graph, struct Node* node, struct NodeList* visited, struct NodeList* rec) {
	if (findTarget(rec, node->info->target) != NULL) 
		return 1;

	if (findTarget(visited, node->info->target) != NULL) 
		return 0;
	
	addTarget(visited, node->info);
	addTarget(rec, node->info);
	for (int i = 0; i < node->info->dpsize; i++) {
		if (node->children[i] != NULL) {
			if ( isCycle(graph, node->children[i], visited, rec) == 1){
			       return 1;
			}
		} else {
			break;
		}			
	}
	
	struct Node* reccurr = rec->head;
	while (strcmp(reccurr->next->info->target, node->info->target) != 0)
		reccurr = reccurr->next;
	
	struct Node* temp = reccurr->next;
	reccurr->next = temp->next;	

	return 0;

}

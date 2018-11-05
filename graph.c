#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct that holds the targets
struct Info {
	int dpsize;
	char* target;
	struct Command* cmds;
	char** dps;
}

// Struct that holds a command
struct Command {
	char* cmd;
	char** args;
}

// Struct for a node in the graph, holding a target and the next node
struct Node {
	struct Info* info;
	struct Node** children;
	struct Node* next;
}

// List of nodes for the graph
struct NodeList {
	int size;
	struct Node *head;
}

// Create a node given the info struct
struct Node* createNode(struct * Info info) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node*));
	newNode->info = info;
	newNode->children = NULL;
	newNode->next = NULL;
	return newNode;
}

// Create a list
struct NodeList* createList(int tsize){
	struct NodeList* newlist = (struct NodeList*)malloc(sizeof(struct NodeList*));
	newlist->head->info = NULL;
	newlist->head->children = NULL;
	newlist->head->next=NULL;
	newlist->size = tsize;
	return newlist;
}

// Add target to list
void addTarget(struct NodeList* list, Info* info){
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
	while (curr->next != NULL && strcmp(curr->next->info->target, target) !- 0) {
		curr = curr_.next;
	}
	return curr->next;
}

// Graph struct holding multiple list of nodes
//struct Graph{
//	int size;
//	struct NodeList* list;
//}

// Initialize graph
//struct Graph* createGraph(int size) {
//	struct Graph *graph  = (struct Graph*)malloc(sizeof(struct * Graph));
//	graph->size = size;
//	graph->list = (struct NodeList*)malloc(size*sizeof(struct NodeList*));
//	for (int i = 0; i < size; i++){
//		graph->list[i].head = NULL;
//	}
//
//	return graph;
//}

// Add an edge between a target and another target in the graph. 
//void addEdge(struct Graph*, int src, struct Info* info) {
//	struct Node* newNode = createNode(info);
//	newNode->next = graph->list[src].head;
//	graph->list[src].head = newNode;
//}

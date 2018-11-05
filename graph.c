#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct that holds the targets
struct Info {
	int dpsize;
	char* target;
	char** cmds;
	char** dps;
}

// Struct for a node in the graph, holding a target and the next node
struct Node {
	struct Info* info;
	struct Node* next;
}

// List of nodes for the graph
struct NodeList {
	struct Node *head;
}

// Graph struct holding multiple list of nodes
struct Graph{
	int size;
	struct NodeList* list;
}

// Create a node by passing in info for the target
struct Node* createNode(struct * Info info) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node*));
	newNode->info = info;
	newNode->next = NULL;
	return newNode;
}

// Initialize graph
struct Graph* createGraph(int size) {
	struct Graph *graph  = (struct Graph*)malloc(sizeof(struct * Graph));
	graph->size = size;
	graph->list = (struct NodeList*)malloc(size*sizeof(struct NodeList*));
	for (int i = 0; i < size; i++){
		graph->list[i].head = NULL;
	}

	return graph;
}

// Add an edge between a target and another target in the graph. 
void addEdge(struct Graph*, int src, struct Info* info) {
	struct Node* newNode = createNode(info);
	newNode->next = graph->list[src].head;
	graph->list[src].head = newNode;
}

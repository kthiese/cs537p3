// Alec Scheele
// Kathryn Thiese

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "runMake.h"

// Constructs the graph
void createGraph(struct Info** targets, int tsize, char* makeTarget) {
	struct NodeList* graph = createList(tsize);
	
	// Add each target to graph, making a linked list of the targets
	for (int i = 0; i < tsize; i++) {
		// Check if a target is listed twice
		if (findTarget(graph, targets[i]->target) != NULL) {
			fprintf(stderr, "ERROR: Target '%s' is listed multiple times.\n", targets[i]->target);
			exit(1);
		}	
		addTarget(graph, targets[i]);
	}
	
	if (strcmp(makeTarget, "") != 0 && findTarget(graph, makeTarget) == NULL){
		fprintf(stderr, "ERROR: '%s' is not a target in the makefile.\n", makeTarget);
		exit(1);	
	}

	// For each node in the graph . . . 
	struct Node* curr = graph->head;
	int v = 0;
	for (int i = 0; i < tsize; i++) {
		curr = curr->next;
		// allocate memory for the children, and for each dependency . . .
		curr->children = (struct Node **)malloc(sizeof(struct Node*) * curr->info->dpsize);
		for (int j = 0; j < curr->info->dpsize; j++){
			// check if the file is also a target, and add it to the children.
			for (int k = 0; k < tsize; k++) {
				if (k == i){
					continue;
				} else if (strcmp(curr->info->dps[j], targets[k]->target) == 0) {
					curr->children[v++] = findTarget(graph, targets[k]->target);
				}
			}
		}
		curr->numchild = v;
		v = 0;
	}
	
	// Checks if graph contains a file. If yes, exit. 
	if (containsCycle(graph) == 1) {
		fprintf(stderr, "ERROR: Makefile contains a cycle.\n");
		exit(1);
	}

	
	// Pass the graph to actually run the makefile. 
	runMake(graph, makeTarget);
}


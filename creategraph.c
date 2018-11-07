#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

// Constructs the graph
int createGraph(struct Info** targets, int tsize) {
	struct NodeList* graph = createList(tsize);
	
	// Add each target to graph, making a linked list of the targets
	for (int i = 0; i < tsize; i++) {
		if (findTarget(graph, targets[i]->target) != NULL) {
			fprintf(stderr, "ERROR: Target '%s' is listed multiple times.\n", targets[i]->target);
			return 1;
		}	
		addTarget(graph, targets[i]);
	}

	// For each node in the graph, 
	struct Node* curr = graph->head;
	int v = 0;
	for (int i = 0; i < tsize; i++) {
		curr = curr->next;
		// Allocated memory for the children, and for each dependency
		curr->children = (struct Node **)malloc(sizeof(struct Node*) * curr->info->dpsize);
		for (int j = 0; j < curr->info->dpsize; j++){
			// Check if the file is also a target, and add it to the children.
			for (int k = 0; k < tsize; k++) {
				if (k == i){
					continue;
				} else if (strcmp(curr->info->dps[j], targets[k]->target) == 0) {
					curr->children[v++] = findTarget(graph, targets[k]->target);
				}
			}
		}
		v = 0;
	}

	if (containsCycle(graph) == 1) {
		fprintf(stderr, "ERROR: Makefile contains a cycle.\n");
		return 1;
	}

	
	// Pass the graph to actually run the makefile. 
//	runMake(graph);

	curr = graph->head;
	for (int i = 0; i < tsize; i++) {
		curr = curr->next;
		printf("%s: ", curr->info->target);
		for (int j = 0; j < curr->info->dpsize; j++) {
			printf("%s ", curr->info->dps[j]);
		}
		printf("\n\tChildren: ");
		for (int j = 0; j < curr->info->dpsize; j++) {
			if (curr->children[j] == NULL) {
				break;
			} else {
				printf("%s ", curr->children[j]->info->target);
			}
		}
		printf("\n");
	}

	return 0;
}


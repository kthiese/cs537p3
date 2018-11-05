#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constructs the graph
int createGraph(Info** targets, int tsize) {
	struct NodeList* graph = createList(tsize);
	
	// Add each target to graph, making a linked list of the targets
	for (int i = 0; i < tsize; i++) {
		addTarget(graph, targets[i]);
	}

	// For each node in the graph, add the children.
	struct Node* curr = graph->head;
	int v = 0;
	for (i = 0; i < tsize; i++) {
		curr = curr->next;
		curr->children = (struct Node *)malloc(sizeof(struct Node*) * curr->info->dpssize);
		for (int j = 0; j < curr->info->dpsize; j++){
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

//	// For each target
//	for (int i = 0; i < tsize; i++){
//		// check if one of the dependencies
//		for (int j = 0; j < targets[i]->dpsize; j++) {
//		       // is also a target. If yes, then add an edge from that
//		       // target to that 'dependency' (which is actually another target)
//			for (int k = 0; k < tsize; k++) {
//				if (k == i) {
//					continue;
//				} else if (strcmp(targets[k].target, targets[i]->dps[j]) == 0) {
//					addEdge(graph, i, targets[k]);
//				}
//		       }
//		}	       
//	}
	
	// Pass the graph to actually run the makefile. 
	runMake(graph);
}

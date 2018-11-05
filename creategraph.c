#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constructs the graph
int createGraph(Info** targets, int tsize) {
	struct Graph* graph = createGraph(tsize);
	
	// For each target
	for (int i = 0; i < tsize; i++){
		// check if one of the dependencies
		for (int j = 0; j < targets[i]->dpsize; j++) {
		       // is also a target. If yes, then add an edge from that
		       // target to that 'dependency' (which is actually another target)
			for (int k = 0; k < tsize; k++) {
				if (k == i) {
					continue;
				} else if (strcmp(targets[k].target, targets[i]->dps[j]) == 0) {
					addEdge(graph, i, targets[k]);
				}
		       }
		}	       
	}
	
	// Pass the graph to actually run the makefile. 
	runMake(graph);
}

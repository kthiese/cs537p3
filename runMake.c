// Alec Scheele
// Kathryn Thiese

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include "graph.h"
#include <time.h>
#include "runMake.h"

// Pass in a graph to run Make. 
void runMake(struct NodeList* graph, char* makeTarget) {
	int size = graph->size;
	struct Node* curr;
	if (strcmp(makeTarget, "") != 0) {
		curr = findTarget(graph, makeTarget);
	} else {
		curr = graph->head->next;
	}

	struct NodeList* visited = createList(size);
	run(graph, curr, visited);
}

// Recursive function for running make
void run(struct NodeList* graph, struct Node* curr, struct NodeList* visited) {
	addTarget(visited, curr->info);
	for (int i = 0; i < curr->numchild; i++) {
			if (findTarget(visited, curr->children[i]->info->target) != NULL) {
			} else {
				run(graph, curr->children[i], visited);

			}
	}	
	struct stat target;
	struct stat dp;
	// Check for each dependency of a target, check the modifcation time.
	if (curr->info->dpsize != 0) {
		for (int i = 0; i < curr->info->dpsize; i++) {
			if (stat(curr->info->dps[i], &dp) != 0) {
				if (curr->info->cmdsize != 0) {
					struct Node* n = findTarget(graph, curr->info->dps[i]);
					if (n != NULL && n->info->dpsize != 0) { 
						fprintf(stderr, "ERROR: File '%s' not found during execution.\n", curr->info->dps[i]);
						exit(1);
					}
				}
			} 
			if (stat(curr->info->target, &target) != 0 || (curr->info->cmdsize != 0 && difftime(dp.st_mtime, target.st_mtime) > 0)) {
				pid_t child_pid;
				int child_status;
				for (int j = 0; j < curr->info->cmdsize; j++) {
					if (curr->info->cmds[j]->cmd != NULL) {
						child_pid = fork();
						if (child_pid == -1) {
							fprintf(stderr, "ERROR: Could not fork child.\n");
							exit(1);
						}
						else if (child_pid == 0) {
							for (int k = 0; k < curr->info->cmds[j]->argsize-1; k++) {
								printf("%s ", curr->info->cmds[j]->args[k]);
							}
							printf("\n");
							execvp(curr->info->cmds[j]->cmd, curr->info->cmds[j]->args);
							fprintf(stderr, "ERROR: Command failed.\n");
							exit(0);
							
						} else {
							pid_t tpid; 
							do {
								tpid = wait(&child_status);
							}while (tpid != child_pid);
						}
					}
				}
			}
		}
	} else 
	// If no dependencies, we can just automatically run the commands. 
	{
		pid_t child_pid;
		int child_status;
		for (int j = 0; j < curr->info->cmdsize; j++) {
			if (curr->info->cmds[j]->cmd != NULL) {
				child_pid = fork();
				if (child_pid == -1) {
					fprintf(stderr, "ERROR: Could not fork child.\n");
					exit(1);
				}
				else if (child_pid == 0) {
					for (int k = 0; k < curr->info->cmds[j]->argsize-1; k++) {
						printf("%s ", curr->info->cmds[j]->args[k]);
					}
					printf("\n");
					execvp(curr->info->cmds[j]->cmd, curr->info->cmds[j]->args);
					fprintf(stderr, "ERROR: Command failed.\n");
					exit(1);

				} else {
					pid_t tpid; 
					do {
						tpid = wait(&child_status);
					}while (tpid != child_pid);
				}
			}
		}
	}
}

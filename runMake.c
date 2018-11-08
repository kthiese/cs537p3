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
void runMake(struct NodeList* graph) {
	int size = graph->size;
	struct Node* curr = graph->head->next;

	struct NodeList* visited = createList(size);
	printf("HELLO! IN RUNMAKE!\n");
	run(curr, visited);
}

// Recursive function for running make
void run(struct Node* curr, struct NodeList* visited) {
	addTarget(visited, curr->info);
	printf("IN RUN!\n");
	for (int i = 0; i < curr->info->dpsize; i++) {
	       if (curr->children[i] != NULL) {
			if (findTarget(visited, curr->children[i]->info->target) == NULL) {
				break;
			} else {
				run(curr->children[i], visited);

			}
		} else {
			break;
	       	}		
	}
	
	struct stat* target;
	struct stat* dp;
	target = malloc(sizeof(struct stat));
	dp = malloc(sizeof(struct stat));
	// Check for each dependency of a target . . .
	if (curr->info->dpsize != 0) {
		for (int i = 0; i < curr->info->dpsize; i++) {
			if (stat(curr->info->dps[i], dp) != 0) {
				fprintf(stderr, "ERROR: File '%s' not found during execution.\n", curr->info->dps[i]);
				exit(0);
			} 
			if (stat(curr->info->target, target) != 0) {
				fprintf(stderr, "ERROR: Target file '%s' not found during execution.\n", curr->info->target);
			}
			// if that dependency has a later modification time than the target. 
			// If so, run the commands for the target. 	
			if (difftime(dp->st_mtime, target->st_mtime) > 0) {
				pid_t child_pid;
				int child_status;
				for (int j = 0; j < curr->info->cmdsize; j++) {
					child_pid = fork();
					if (child_pid == 0) {
						for (int k = 0; k < curr->info->cmds[j]->argsize; k++) {
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
				break;
			}
		}
	} else {
		printf("ZERO DEPENDENCIES!\n");
		pid_t child_pid;
		int child_status;
		for (int j = 0; j < curr->info->cmdsize; j++) {
			child_pid = fork();
			if (child_pid == 0) {
				for (int k = 0; k < curr->info->cmds[j]->argsize; k++) {
					printf("%s ", curr->info->cmds[j]->args[k]);
				}
				printf("\n");
				execvp(curr->info->cmds[j]->cmd, curr->info->cmds[j]->args);
				printf("EXECUTING!\n");
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
	

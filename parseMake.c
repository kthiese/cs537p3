// Alec Scheele
// Kathryn Thiese

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include "graph.h"
#include "creategraph.h"

// constant for buffer size
const int bufSize = 1024;
int tsize;
struct Info** CreateTargets(char* fileName)
{
	// list of targets to return
	struct Info** targetList;
	
	// file pointer
	FILE *ptr;

	// int to be returned by fgetc
	int c;

	// array of lines
	char** arrayOfLines = (char **)malloc(sizeof(char*)*bufSize);
	for (int i = 0; i < bufSize; i++){
		arrayOfLines[i] = (char *)malloc(sizeof(char)*bufSize);
	}
	
	// Open file
	if ((ptr = fopen(fileName, "r")) == NULL)
	{
		if ((ptr = fopen("Makefile", "r")) == NULL){
			// if cannot open file, print error and exit
			fprintf(stderr, "ERROR: Cannot open file.\n");
			exit(1);
		}
	} 

	// indexes for line and character numbers
	int lineNo = 0;
	int charNo = 0;

	do // reading contents of file line by line, checking line length
	{
		c = fgetc(ptr);
		if (c != '\n' && c != EOF) {
			if (charNo < bufSize) {
				arrayOfLines[lineNo][charNo++] = c;
			} else {
				fprintf(stderr, "ERROR: Line %i is too long.\n", lineNo+1);	
				exit(1);
			}
		} else if (c == '\n') {
			if (charNo < bufSize) {
				arrayOfLines[lineNo][charNo++] = c;
				lineNo++;
				charNo = 0;
			} else {
				fprintf(stderr, "ERROR: Line %i is too long.\n", lineNo+1);
				exit(1);	
			}
		} else if (c == EOF) {
			break;
		}
	} while (1);

	fclose(ptr);

	targetList = (struct Info**)malloc(sizeof(struct Info*)*lineNo);	

	// start going though arrayOfLines and determine if line is valid, or if is comment
	// if valid and not comment check if target or if command

	
	// iterate through array of lines
	tsize = 0;
	for(int i = 0; i < lineNo; i++)
	{
		// check for comment or new line if so skipconst char *
		if(!(arrayOfLines[i][0] == '\n' || arrayOfLines[i][0] == '#'))
		{
			// Check for invalid line, if line is invalid print line #, line contents and stderror
			if(arrayOfLines[i][0] == ' ')
			{
				fprintf(stderr, "ERROR: Line %i should be a target line.\n", i+1);
				fprintf(stderr, "'%s'\n", arrayOfLines[i]);
				exit(1);
			}

			// either target line or command line
			else	
			{
				// check if a target line
				if(arrayOfLines[i][0] != '\t')
				{
					struct Info* tempTarget = (struct Info*)malloc(sizeof(struct Info));
					
					// TARGET name variables
					const char* cptr = strchr(arrayOfLines[i], ':');
					int targetNameLength;
					char* tempTargetName;
					if (cptr) {
						targetNameLength = (cptr-arrayOfLines[i]);
						tempTargetName = (char*)malloc(sizeof(char)*targetNameLength+1);
						memset(tempTargetName, 0, targetNameLength+1);
					} else {
						fprintf(stderr, "ERROR: Line %i does not start with a tab, so it should be a target line.\n", i+1);
						exit(1);
					}

					// get TARGET NAME : iterate through each character in the line until colon is reached and set target name
					for(int j = 0; j < bufSize; j++)
					{
						if(arrayOfLines[i][j] == ':')
						{
							break;
						}
						tempTargetName[j] = arrayOfLines[i][j];
					}
					
					// Trim trailing whitespace
					char* end = tempTargetName + strlen(tempTargetName) - 1;
  					while(end > tempTargetName && isspace((unsigned char)*end)) end--;
					end[1] = '\0';

					// Set target name in struct Info.
					tempTarget->target = tempTargetName;
					// DEPENDENCIES variables
					// get dependancies after target name
					char** tempDepList = (char**)malloc(sizeof(char*)*bufSize);
					for (int k = 0; k < bufSize; k++) {
						tempDepList[k] = (char*)malloc(bufSize*sizeof(char));
					}
					char* tempDeps = (char*)malloc(sizeof(char)*bufSize);
					int dCharIndex = 0;
	
					// is set to 1 if previous char was a space
	
					// get DEPENDENCIES and add deps to tempDepList
					for(int j = targetNameLength + 1; j < bufSize; j++)
					{
						if (arrayOfLines[i][j] == '\n') {
							break;							
						} else{
							tempDeps[dCharIndex++] = arrayOfLines[i][j];
						}
					}
					char *sdeps = strtok(tempDeps, " ");
					int count = 0;
					while (sdeps != NULL){
						tempDepList[count++] = sdeps;
						sdeps = strtok(NULL, " ");
					} 
					//Set target dependencies and dependencies size.
					tempTarget->dps = tempDepList;
					tempTarget->dpsize = count;

					// CMD variables
					char** tempCmdList = (char**)malloc(sizeof(char*)*bufSize);
					for (int k = 0; k < bufSize; k++) {
						tempCmdList[k] = (char*)malloc(bufSize*sizeof(char));
					}
					int cNoIndex = 0;

					// check if line imediately after targetline is cmdline, if so get cmdline broken by spaces
					while(arrayOfLines[i+1][0] == '\t' || arrayOfLines[i+1][0] == '\n')
					{
						if (arrayOfLines[i+1][0] == '\n') {
							i = i+1;
							continue;
						}
						int cCharIndex = 0;
			
						i = i + 1;
			
						for(int j = 1; j < bufSize; j++)
						{
							if (arrayOfLines[i][j] == '\n')
								break;
							tempCmdList[cNoIndex][cCharIndex] = arrayOfLines[i][j];
							cCharIndex = cCharIndex + 1;
						}
						
						cNoIndex = cNoIndex + 1;
					}
					
					tempTarget->cmds = (struct Command**)malloc(sizeof(struct Command*)*cNoIndex);
					tempTarget->cmdsize = cNoIndex;
					for (int k = 0; k < cNoIndex; k++) {
						tempTarget->cmds[k] = (struct Command*)malloc(sizeof(struct Command));
						char** tempArgs = (char **)malloc(sizeof(char*)*bufSize);						
						char *sargs = strtok(tempCmdList[k], " ");
						int count = 0;
						while (sargs != NULL) {
							tempArgs[count++] = sargs;
							sargs = strtok(NULL, " ");
						}
						tempArgs[count++] = '\0';
						tempTarget->cmds[k]->args = (char **)malloc(sizeof(char*)*bufSize);
						tempTarget->cmds[k]->argsize = count;
						for (int l = 0; l < count; l++) {
							tempTarget->cmds[k]->args[l] = tempArgs[l];
						}
						tempTarget->cmds[k]->cmd = *tempArgs;
					}
					targetList[tsize++] = tempTarget;
				}

				else // line is not a target line
				{
					fprintf(stderr, "Line %i is not a target line when it should be.\n", i+1);
					exit(1);
				}
			}
		}
	}
	return targetList;
}

int main(int argc, char* argv[])
{
	int f = 0;
	int opt = 0;
	if ((opt = getopt(argc, argv, "f:")) != -1) 
	{
		f = 1;
		switch(opt)
		{
			case 'f':
				if (argc > 4)
				{
					fprintf(stderr, "ERROR: Too many arguments for 'make'.\n");
					exit(1);
				}

				struct Info** targetList = CreateTargets(optarg);
				
				if (argc == 3)
				{
					createGraph(targetList, tsize, "");
				}
				else
				{
					createGraph(targetList, tsize, argv[3]);
				}
		}
	}

	if (f == 0)
	{
		if (argc > 2)
		{
			fprintf(stderr, "ERROR: 'make' only takes one or zero arguments.\n");
			exit(1);
		}

		struct Info** targetList = CreateTargets("makefile");

		if (argc == 1) 
		{
			createGraph(targetList, tsize, "");
		}
		else
		{
			createGraph(targetList, tsize, argv[1]);
		}
	}
}


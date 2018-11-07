#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>

// constant for buffer size
const int bufSize = 1024;

// Struct that holds the targets
struct Info {
	int dpsize;
	char* target;
	struct Command* cmds;
	char** dps;
};

// Struct that holds a command
struct Command {
	char* cmd;
	char** args;
};

struct Info** CreateTargets(char* fileName)
{
	// list of targets to return
	struct Info** targetList;
	targetList = (struct Info**) malloc (sizeof(struct Info**));
	

	// number of targets
	int numTargets = 0;
	
	// file pointer
	FILE *ptr;

	// int to be returned by fgetc
	int c;

	// buffer to hold contents of file
	char lines[bufSize];

	// array of lines
	char arrayOfLines[bufSize][bufSize];

	// index for buffer
	int lineIndex = 0;

	
	if ((ptr = fopen(fileName, "r")) == NULL)
	{
		// if cannot open file, print error and exit
		printf("Error opening file");

		exit(1);
	} 
	do // reading contents of file line by line
	{
		c = fgetc(ptr);
		lines[lineIndex] = c;
		lineIndex = lineIndex + 1;

		if(feof(ptr))
		{
			break;
		}

	} 
	while (1);
	
	fclose(ptr);

	// finished reading make into line[] and ptr closed
	// indexes for line and character numbers
	int lineNo = 0;
	int charNo = 0;

	// breaking file into seperate lines
	for(int i = 0; i < bufSize; i++)
	{
		if(lines[i] == EOF)
		{
			//printf("%d", lineNo);
			break;
		}
		if(lines[i] == '\n')
		{
			arrayOfLines[lineNo][charNo] = lines[i]; 
			lineNo = lineNo + 1;
			charNo = 0;
		}
		else
		{
			arrayOfLines[lineNo][charNo] = lines[i];
			charNo = charNo + 1;
		}

	}

	// start going though arrayOfLines and determine if line is valid, or if is comment
	// if valid and not comment check if target or if command

	
	// iterate through array of lines
	for(int i = 0; i < lineNo; i++)
	{
		// check for comment or new line if so skip
		if(!(arrayOfLines[i][0] == '\n' || arrayOfLines[i][0] == '#'))
		{
			// TODO: check for invalid line, if line is invalid print line #, line contents and stderror
			if(arrayOfLines[i][0] == ' ')
			{
				printf("invalid line: %d\n", i+1);
				for(int j = 0; j < bufSize; j++)
				{
					if(arrayOfLines[i][j] == '\0')
						break;
					printf("%c",arrayOfLines[i][j]);
				}
				exit(1);
			}

			// either target line or command line
			else	
			{
				// check if a target line
				if(arrayOfLines[i][0] != '\t')
				{


					numTargets = numTargets + 1;
					// TARGET name variables
					char tempTargetName[bufSize];
					memset(tempTargetName, 0, sizeof tempTargetName);
					int targetNameLength = 0;

					// get TARGET NAME : iterate through each character in the line until colon is reached and set target name
					for(int j = 0; j < bufSize; j++)
					{
						if(arrayOfLines[i][j] == ':')
						{
							targetNameLength = j;
							break;
						}
		
						tempTargetName[j] = arrayOfLines[i][j];
					}
					
				
					// DEPENDENCIES variables
					// get dependancies after target name
					char tempDepList[bufSize][bufSize];
					memset(tempDepList, 0, sizeof tempDepList);
					int noDeps = 0;
					int dNoIndex = 0;
					int dCharIndex = 0;
	
					// is set to 1 if previous char was a space
					int spaceFlag = 0;
	
					// get DEPENDENCIES and add deps to tempDepList
					for(int j = targetNameLength + 1; j < bufSize; j++)
					{
						if(arrayOfLines[i][j] != ' ')
						{
							spaceFlag = 0;
							tempDepList[dNoIndex][dCharIndex] = arrayOfLines[i][j];
							dCharIndex = dCharIndex + 1;
						}
						if(arrayOfLines[i][j] == ' ')
						{
							if(spaceFlag == 0)
							{
								dCharIndex = 0;
								dNoIndex = dNoIndex + 1;
								noDeps = dNoIndex;
								spaceFlag = 1;
							}
						}
					}


					// CMD variables
					char tempCmdList[bufSize][bufSize];
					memset(tempCmdList, 0, sizeof tempCmdList);
					int noCmds = 0;
					int cCharIndex = 0;
					int cNoIndex = 0;

					// check if line imediately after targetline is cmdline or blank or comment, if so get cmdline broken by spaces or skip
					while(arrayOfLines[i+1][0] == '\t' || arrayOfLines[i+1][0] == '\n' || arrayOfLines[i+1][0] == '#')
					{
						// next line is cmd line
						if(arrayOfLines[i+1][0] == '\t')
						{
							int cCharIndex = 0;
			
							i = i + 1;
			
							for(int j = 1; j < bufSize; j++)
							{
								tempCmdList[cNoIndex][cCharIndex] = arrayOfLines[i][j];
								cCharIndex = cCharIndex + 1;
							}
							
							cNoIndex = cNoIndex + 1;
							noCmds = cNoIndex;
						}
						else
						{
							i = i + 1;
						}
					}

					// realloc space for targetList
					targetList = realloc(targetList, numTargets*sizeof(struct Info**));

					//TODO: add Info struct to target list

					// create Info struct to hold data
					struct Info * target1;
					target1 = malloc(sizeof(struct Info));

					// set target name in target info struct
					target1->target = tempTargetName;

					// set number of DEPENDENCES
					target1->dpsize = noDeps;
					
					// add target to list
					*(targetList + numTargets * sizeof(struct Info)) = target1;

					// TESTING PARSER SO FAR
					/*
					printf("target: ");
					for(int j = 0; j < targetNameLength; j++)
					{
						printf("%c", tempTargetName[j]);
					}
					printf("\n");
						
					printf("deps: ");
					//printf("\n");
					//printf("%d", noDeps);
					for(int j = 0; j <= noDeps; j++)
					{
						printf("\n");
						for(int l = 0; l < bufSize; l++)
						{
							printf("%c", tempDepList[j][l]);
						}
					}
					
					printf("cmdlines: ");
					//printf("\n");
					for(int j = 0; j <= noCmds; j++)
					{
						for(int l = 0; l < bufSize; l++)
						{
							printf("%c", tempCmdList[j][l]);
						}
						//printf("\n");
					}
					*/
					// END PARSER TESTING
				}

				else // line is not a target line
				{
					// TODO: print error b/c make file must begin with target line
					printf("make must begin with target line");
					exit(1);
				}
			}
		}
	}
	printf("numTargets: %d", numTargets);
}

int main()
{

	struct Info** list = (struct Info**)malloc(sizeof(struct Info**));
	list = CreateTargets("make");

	printf("%s", list[0]->target);
	
	
}


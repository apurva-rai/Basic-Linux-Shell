#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <string>
#include <errno.h>
#include "executive.c"

#define BYTESIZE 256


using namespace std;

typedef enum
{
	regular, pipes, input, output
} status;

typedef struct
{
	char** argv;
	int argc;
	char **env;
	status seeStatus;
	int background;
	char goal[BYTESIZE];
} commandSignal;

commandSignal* parse(char* buffer, char **env)
{
	int argc = 0;
	int background;
	commandSignal* current;
	char raw[BYTESIZE];
	char* temp;
	char* tokenize;
	char** argv = (char**) malloc(sizeof(char*) * BYTESIZE);
	char** argv2;
	current = (commandSignal*) malloc(sizeof(commandSignal));
	strcpy(raw, buffer);
	tokenize = strtok(raw, " \n");
	while (tokenize != NULL )
	{
		temp = (char*) malloc((strlen(tokenize) * sizeof(char)) + 1);
		memset(temp, '\0', (strlen(tokenize) * sizeof(char)) + 1);
		strcpy(temp, tokenize);
		argv[argc] = temp;
		temp = NULL;
		tokenize = strtok(NULL, " \n");
		argc++;
	}
	if (argc != 0)
	{
		if (strcmp(argv[argc - 1], "&") == 0)
		{
			background = 1;
			argv[argc - 1][0] = '\0';
		}
		else
		{
			background = 0;
		}
	}
	argv2 = (char**) malloc(sizeof(char*) * (argc + 1));
	int i = 0;
	for (i = argc; i < BYTESIZE - 1; i++)
	{
		argv[argc] = "\0";
	}
	(*current).argc = argc;
	(*current).background = background;
	(*current).argv = argv;
	(*current).seeStatus = regular;
	(*current).env = env;
	return current;
}

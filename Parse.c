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

typedef enum {
	regular, pipes, in, out
} type;

typedef struct{
	char** argv;
	int argc;
	char **env;
	type command_type;
	int run_in_background;
	char target[BYTESIZE];
} command;

command* parse_raw_input(char* buffer, char **env) {

	//Temporary stuff
	char raw[BYTESIZE];
	char* temp;
	char* token;
	char** argv = (char**) malloc(sizeof(char*) * BYTESIZE);
	char** argv2;
	int argc = 0;
	int run_in_background;

	command* current_cmd;

	current_cmd = (command*) malloc(sizeof(command));


	//Copy buffer to raw to avoid messing with other stuff
	strcpy(raw, buffer);

	//Tokenize on spaces
	token = strtok(raw, " \n");
	while (token != NULL ) {
		// Malloc a new string
		temp = (char*) malloc((strlen(token) * sizeof(char)) + 1);

		// Zero it out
		memset(temp, '\0', (strlen(token) * sizeof(char)) + 1);

		// Copy the contents of our token into temp
		strcpy(temp, token);

		// Set the argv array to the temp pointer freshly malloced
		argv[argc] = temp;

		// Clean temp pointer
		temp = NULL;

		//Tokenize again
		token = strtok(NULL, " \n");
		argc++;
	}

	// Run detection for background operation
	if (argc != 0) {
		if (strcmp(argv[argc - 1], "&") == 0) {
			run_in_background = 1;
			argv[argc - 1][0] = '\0';
		} else {
			run_in_background = 0;
		}
	}

	//Trims the emoty slots of the argv array to avoid causing headaches
	//when running commands with different amounts of arguments
	//I lost 3 hours until I realized this.
	argv2 = (char**) malloc(sizeof(char*) * (argc + 1));
	int i = 0;
	for (i = argc; i < BYTESIZE - 1; i++) {

		argv[argc] = "\0";
	}

	//Set our command to its contents
	(*current_cmd).argc = argc;
	(*current_cmd).run_in_background = run_in_background;
	(*current_cmd).argv = argv;
	(*current_cmd).command_type = regular;
	(*current_cmd).env = env;

	//Return the pointer to the malloced Command
	return current_cmd;
}

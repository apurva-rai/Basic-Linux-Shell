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

// struct Jobs
// {
//     int pid;
//     int id;
//     char* cmd;
// };
// static int job_count = 0;
// static Jobs jobs[100];
// static char* env;
// static char* dir;
//
// void display_jobs() {
//     int i;
//     printf("\nActive jobs:\n");
//     printf("| %7s  | %7s | %7s |\n", "Job ID", "PID  ", "Command");
//     for (i=0; i < job_count; i++) {
//         if (kill(jobs[i].pid, 0) == 0) {
//             printf("|  [%7d] | %7d | %7s |\n", jobs[i].id, jobs[i].pid,
//                    jobs[i].cmd);
//         }
//     }
// }
//
// void parse_cmd(char* input) {
//     int i;
//     char* command;
//     int fout = 1994;
//     char* args[20];
//     for (i = 0; i < 20; i++) {
//         args[i] = NULL;
//     }
//     int args_count = 0;
//     char* cur_input = strdup(input);
//     command = strtok(input, " ");
//     while (command != NULL) {
//         args[args_count] = command;
//         command = strtok(NULL, " ");
//         args_count++;
//     }
//     char* just_args[19];
//     for (i = 0; i < 19; i++) {
//         just_args[i] = NULL;
//     }
//     for (i = 1; i < 20; i++) {
//         if (args[i] != NULL) {
//             just_args[i - 1] = args[i];
//         }
//     }
//     char* is_backgrd = strchr(cur_input, '&');
//     char* bg_command = strdup(input);
//     bg_command[strlen(input) - 1] = 0;
//     char* is_pipe = strchr(cur_input, '|');
//     char* filedir_in = strchr(cur_input, '<');
//     char* filedir_out = strchr(cur_input, '>');
//     char* kill_proc = strstr(cur_input, "kill");
//     int change_dir = strcmp("cd", args[0]);
//     int set = strcmp(args[0], "set");
//     int jobs_com = strcmp(args[0], "jobs");
//
//     if (change_dir == 0) {
//         cd(args[1]);
//     }
//     else if (set == 0) {
//         setPath(args[1]);
//     }
//     else if (jobs_com == 0) {
//         display_jobs();
//     }
//     else if (is_backgrd != NULL) {
//         int status;
//         pid_t pid;
//         pid_t sid;
//         pid = fork();
//         if (pid == 0) {
//             sid = setsid();
//             if (sid < 0 ) {
//                 fprintf(stderr, "Could not create the new process\n");
//                 exit(0);
//             }
//             printf("New process with pid %d running out of %d processes\n", getpid(), job_count + 1);
//             parse_cmd(bg_command);
//             printf("The process with pid %d has finished\n", getpid());
//             kill(getpid(), -9);
//             exit(0);
//         }
//         else {
//             struct Jobs new_job = {.pid = pid, .id = job_count, .cmd = bg_command};
//             jobs[job_count] = new_job;
//             job_count++;
//             //while(waitid(pid, NULL, WNOHANG | WEXITED) > 0) {}
//         }
//     }
//     else if (is_pipe != NULL) {
//         char* part = strtok(cur_input, "|\0");
//         char* first_cmd = part;
//         part = strtok(NULL, "\0");
//         char* second_cmd = part;
//         int spipe[2];
//         int status;
//         pipe(spipe);
//         pid_t pid;
//         pid_t pid2;
//         pid = fork();
//         if (pid == 0) {
//             dup2(spipe[1], STDOUT_FILENO);
//             parse_cmd(removeWhitespace(first_cmd));
//             close(spipe[0]);
//             close(spipe[1]);
//             exit(0);
//         }
//         pid2 = fork();
//         if (pid2 == 0) {
//             dup2(spipe[0], STDIN_FILENO);
//             parse_cmd(removeWhitespace(second_cmd));
//             close(spipe[0]);
//             close(spipe[1]);
//             exit(0);
//         }
//     }
//     else if (filedir_in != NULL) {
//         char* cmd_f_file = NULL;
//         char* new_args[20];
//         int i;
//         for (i = 0; i < 20; i++) {
//             new_args[i] = NULL;
//         }
//         size_t len = 0;
//         ssize_t read;
//         FILE* file_d;
//         int status;
//         pid_t pid;
//         pid = fork();
//         if (strcmp("quash", args[0]) == 0) {
//             file_d = fopen(args[2], "r");
//             do {
//                 read = getline(&cmd_f_file, &len, file_d);
//                 if (pid == 0) {
//                     parse_cmd(removeWhitespace(cmd_f_file));
//                 } else {
//                     waitpid(pid, &status, 0);
//                     if(status == 1) {
//                         fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
//                     }
//                 }
//             } while (read != -1);
//             fclose(file_d);
//         } else {
//             file_d = fopen(args[2], "r");
//             int x = 0;
//             int position = 0;
//             while (position == 0) {
//                 if (strcmp("<",args[x]) == 0) {
//                     position = x;
//                 }
//                 x++;
//             }
//             while ((read = getline(&cmd_f_file, &len, file_d)) == -1) {
//                 char* temp_i = strtok(cmd_f_file, " \0");
//                 args[x] = temp_i;
//                 x++;
//                 printf("%s\n",args[x]);
//             }
//             if (pid == 0) {
//                 execute(args);
//             } else {
//                 waitpid(pid, &status, 0);
//                 if(status == 1) {
//                     fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
//                 }
//             }
//         }
//     }
//     else if (filedir_out != NULL) {
//         int i;
//         int position = 0;
//         while (position == 0) {
//             if (strcmp(">",args[i]) == 0) {
//                 position = i;
//             }
//             i++;
//         }
//         int status;
//         pid_t pid;
//         pid = fork();
//         if (pid == 0) {
//             char* filename = args[position + 1];
//             fout = open(filename, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IRGRP | S_IWGRP | S_IWUSR);
//             dup2(fout, STDOUT_FILENO);
//             char* n_input = strtok(input, ">");
//             parse_cmd(removeWhitespace(n_input));
//             close(fout);
//             exit(0);
//         } else {
//             waitpid(pid, &status, 0);
//             if(status == 1) {
//                 fprintf(stderr, "%s\n", "Darn ,the pokemon fled...\n");
//             }
//         }
//     }
//     else if (kill_proc != NULL) {
//         int pid = (int) strtol(args[2], NULL, 0);
//         int signal = (int) strtol(args[1], NULL, 0);
//         printf("Killing pid %d with signal %d\n", pid, signal);
//         kill(pid, -9);
//     }
//     else {
//         execute(args);
//     }
// }

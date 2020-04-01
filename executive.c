#pragma once
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
#include <algorithm>
#include <time.h>



using namespace std;

void executive1(char ***argv, int background, char **env)
{
	int fds;
	pid_t pid;
	pid = fork();
	if (pid == -1)
    {
        perror("fork");
        exit (1);
	}
    else if (pid==0)
    {
		if (execvpe(**argv, *argv, env)==-1)
        {
			printf("error: %s\n",strerror(errno));
			exit(1);
		}
	}
    else
    {
		if (background == 0)
        {
			while (wait(&fds) != pid)
            {
                //waits till the pid is not equal to fds
            }
		}
        else
        {
			printf("[1] %d\n",pid);
		}
	}
	return;
}



void cd(char* buff)
{
	int c =strlen(buff);
	if (buff[c-1]=='\n')
    {
		buff[c-1] = '\0';
	}
	if ((strcmp(buff,"cd")==0) || (strcmp(buff,"cd ")==0))
	{
		chdir(getenv("HOME"));
	}
	else
	{
		if (strncmp(buff,"cd ",3)==0)
        {
			buff+=3;
		}
		if(chdir(buff) == -1)
		{
			printf("Invalid path name.\n");
		}
	}
}


int setPath(char* str)
{
    char* set = strtok(str, "="); //https://www.tutorialspoint.com/c_standard_library/c_function_strtok.htm
    char* tYpe = set;
    set = strtok(NULL, "\0");
    char* path = set;
    if ((setenv(tYpe,path,1)) == - 1)
	{
        printf("%s was not set.\n", tYpe);
    }
    return 1;
}

char* removeWhitespace(char* str)
{
    char *buff;
    while (isspace(*str)) //https://www.programiz.com/c-programming/library-function/ctype.h/isspace
	{
		str++;
	}
    if (*str == 0)
	{
		return str;
	}
    buff = str + strlen(str) - 1;
    while (buff > str && isspace(*buff))
	{
		buff--;
	}
    *(buff + 1) = 0;
    return str;
}


// void execute(char** str)
// {
//     int status;
//     pid_t pid;
//     pid = fork();
//     if (pid == 0)
// 	{
//         if (strlen(str[0]) > 0)
// 		{
//             if (execvp(str[0], str) < 0)
// 			{
//                 fprintf(stderr, "Invalid command\n");
//                 exit(0);
//             }
//         }
// 		else
// 		{
//             if (execvp(str[0], NULL) < 0)
// 			{
//                 fprintf(stderr, "Invalid command\n");
//                 exit(0);
//             }
//         }
//     }
//     else
// 	{
//         waitpid(pid, &status, 0);
//         if (status == 1)
// 		{
//             fprintf(stderr, "%s\n", "Poo poo\n");
//         }
//     }
// }

// pipe output to file
void execute_to_file(char ***argv, int run_in_background, char **env, char* file_out_path){
	int status;
	pid_t pid;
	pid = fork();
	if (pid < 0){
		// thread creation failed
		printf("Error with fork");
		exit(1);
	} else if (pid==0){
		// child thread
		errno = 0;
		FILE *file_out;
		file_out = fopen(file_out_path, "w");
		dup2(fileno(file_out), STDOUT_FILENO);
		fclose(file_out);
		if (execvpe(**argv, *argv, env)<0){
			printf("error replacing process image: %s\n",strerror(errno));
			exit(1);
		}
	} else {
		// parent thread
		if (run_in_background == 0){
			while (wait(&status) != pid){}
		} else {
			printf("[1] %d\n",pid);
		}
	}
	return;
}


// take input from file
void execute_from_file(char ***argv, int run_in_background, char **env, char* file_in_path){
	int status;
	pid_t pid;
	pid = fork();
	if (pid < 0){
		// thread creation failed
		printf("Error with fork");
		exit(1);
	} else if (pid==0){
		// child thread
		errno = 0;
		FILE *file_in;
		/* I'm a little lost. Why do we open a file, dupe STDIN, then immediately close the file again?
		*  This doesn't look like it should actually do anything. */
		file_in = fopen(file_in_path, "r");
		dup2(fileno(file_in), STDIN_FILENO);
		fclose(file_in);
		if (execvpe(**argv, *argv, env)<0){
			printf("error replacing process image: %s\n",strerror(errno));
			exit(1);
		}
	} else {
		// parent thread
		if (run_in_background == 0){
			while (wait(&status) != pid){}
		} else {
			printf("[1] %d\n running in background",pid);
		}
	}
	return;
}


// pipe only
void execute_pipe(char ***argv1, int run_in_background1, char **env1, char ***argv2, int run_in_background2, char **env2){
	int status1, status2;
	pid_t pid1, pid2;
	// initialize pipe
	int pipefd[2];
	pipe(pipefd);

	pid1 = fork();
	if (pid1 < 0){
		// thread creation failed
		printf("Error with fork");
		exit(1);
	} else if (pid1 == 0) {
		// child thread
		errno = 0;
		// close read end of pipe
		close(pipefd[0]);

		// redirect output to pipe
		dup2(pipefd[1], STDOUT_FILENO);
		if (execvpe(**argv1, *argv1, env1) < 0){
			printf("error replacing process image: %s\n",strerror(errno));
			exit(1);
		}
	} else {
		// parent thread
		// first process won't run in background, but can't hurt to check
		if (run_in_background1 == 0){
			while (wait(&status1) != pid1){}
		} else {
			printf("[1] %d\n running in background",pid1);
		}
	}

	pid2 = fork();
	if (pid1 < 0){
		// thread creation failed
		printf("Error with fork");
		exit(1);
	} else if (pid1 == 0) {
		// child thread
		errno = 0;
		// close read end of pipe
		close(pipefd[1]);

		// redirect output to pipe
		dup2(pipefd[0], STDIN_FILENO);
		if (execvpe(**argv2, *argv2, env2) < 0){
			printf("error replacing process image: %s\n",strerror(errno));
			exit(1);
		}
	} else {
		// parent thread
		if (run_in_background2 == 0){
			while (wait(&status2) != pid2){}
		} else {
			printf("[1] %d\n running in background",pid2);
		}
	}

	// close pipes for parent
	close(pipefd[0]);
	close(pipefd[1]);
}

void execute(char ***argv, int run_in_background, char **env) {
	int status;
	pid_t pid;

	pid = fork();
	if (pid < 0){
		// thread creation failed
		printf("Error with fork");
		exit(1);
	} else if (pid==0){
		// child thread
		errno = 0;
		if (execvpe(**argv, *argv, env)==-1){
			printf("error replacing process image: %s\n",strerror(errno));
			exit(1);
		}
	} else {
		// parent thread
		if (run_in_background == 0){
			while (wait(&status) != pid){}
		} else {
			printf("[1] %d\n",pid);
		}
	}
	return;
}

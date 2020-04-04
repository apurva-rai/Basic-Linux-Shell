#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <string.h>
#include <strings.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>
#include <string>
#include <errno.h>
#include <algorithm>
#include <time.h>
#define true 1
#define false 0

using namespace std;


struct Job {
	pid_t pid;
	int id;
	char cmd [300];
};

static struct Job jobs[64];
int job_count = 0;
void inputParse(char* command);


void runBackground(char* command)
{
	pid_t pid;
	int status;
	pid = fork();
	int j, commandlen = strlen(command);
	for (int i=j=0; i < commandlen; i++)
	{
		if(command[i] != '&')
		{
			command[j++] = command[i];
		}
	}
	command[j-1] = '\0';
	if(pid < 0)
	{
		printf("Error!!");
		exit(0);
	}
	if(pid == 0){
		printf("\n[%d] %d Job running in background\n", job_count, getpid());
		inputParse(command);
		printf("\n[%d] %d Job finished %s\nQUASH: %s : ", job_count, getpid(), command, getcwd(NULL,1024));
	}
	else
	{
		struct Job new_job;
		new_job.pid = pid;
		new_job.id = job_count;
		strcpy(new_job.cmd,command);
		jobs[job_count] = new_job;
		job_count++;
	}
	waitpid(pid, &status, WNOHANG);
}


void inputParse(char* command)
{
	pid_t pid;
	pid = fork();
	int status;
	char* args;
	if (pid == 0)
	{
		while ((command[0]) == ' ')
		{
			command++;
		}
		command[strlen(command)] = '\0';
		if (index(command, ' ') == NULL)
		{
			args = NULL;
		}
		else
		{
			args = index(command, ' ');
			args++;
			if (args[0] == '\0')
			{
				command[strlen(command)-1] ='\0';
				args = NULL;
			}
			else
			{
				args[strlen(args)] = '\0';
			}
		}
		if (args == NULL)
		{
			int success = execlp(command, command, NULL);
			if (success == -1)
			{
				exit(0);
			}
		}
		else
		{
			char* action;
			int i = 0;
			action = strtok(command, " ");
			action[strlen(action)] = '\0';
			args[strlen(args)] = '\0';
			int success = execlp(action, action, args, (char *)NULL);
			if (success == -1)
			{
				printf("\nInvalid Command\nPerhaps the wrong arguments\nOr executable not found\n");
				exit(0);
			}
		}
	}
	else
	{
		wait(&status);
	}
	waitpid(pid, &status, 0);
}





void executive1(char ***argv, int background, char **env, char* input)
{
	char* bg_command = strdup(input);
	bg_command[strlen(input) - 1] = 0;
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
		errno = 0;

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

				  // struct Job new_job = {.pid = pid, .id = job_count, .cmd = bg_command};
				  // jobs[job_count] = new_job;
				  // job_count++;
				// while(wait(0))
				// {
				//
				// }
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



void toFile(char ***argv, int background, char **env, char* path)
{
	pid_t pid;
	int type;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit (1);
	}
	 else if (pid==0)
	 {
		errno = 0;
		FILE *file_out;

		file_out = fopen(path, "w");
		dup2(fileno(file_out), STDOUT_FILENO);
		fclose(file_out);
		if (execvpe(**argv, *argv, env)<0)
		{
			printf("error!!: %s\n",strerror(errno));
			exit(1);
		}
	}
	else
	{
		if (background == 0)
		{
			while (wait(&type) != pid)
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



void fromFile(char ***argv, int background, char **env, char* path)
{
	pid_t pid;
	int type;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit (1);
	}
	else if (pid==0)
	{
		errno = 0;

		FILE *file_in;
		file_in = fopen(path, "r");
		dup2(fileno(file_in), STDIN_FILENO);
		fclose(file_in);
		if (execvpe(**argv, *argv, env)<0)
		{
			printf("error!!: %s\n",strerror(errno));
			exit(1);
		}
	}
	else
	{
		if (background == 0)
		{
			while (wait(&type) != pid)
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



void pipeFunc(char ***argv1, int background1, char **env1, char ***argv2, int background2, char **env2)
{
	printf("\n\nPIPE 4 \n\n");
	pid_t pid1, pid2;
	int type1, type2;
	int fds[2];
	pipe(fds);
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit (1);
	}
	else if (pid1 == 0)
	{
		errno = 0;

		close(fds[0]);
		dup2(fds[1], STDOUT_FILENO);
		if (execvpe(**argv1, *argv1, env1) < 0)
		{
			printf("error!!: %s\n",strerror(errno));
			exit(1);
		}
	}
	else
	{
		if (background1 == 0)
		{
			while (wait(&type1) != pid1)
			{
				//waits till the pid is not equal to fds

			}
		}
		else
		{
			printf("[1] %d\n",pid1);
		}
	}
	pid2 = fork();
	if (pid1 == -1)
	{
		perror("fork");
		exit (1);
	}
	else if (pid1 == 0)
	{
		errno = 0;

		close(fds[1]);
		dup2(fds[0], STDIN_FILENO);
		if (execvpe(**argv2, *argv2, env2) < 0)
		{
			printf("error!!: %s\n",strerror(errno));
			exit(1);
		}
	}
	else
	{
		if (background2 == 0)
		{
			while (wait(&type2) != pid2)
			{
				//waits till the pid is not equal to fds

			}
		}
		else
		{
			printf("[1] %d\n running in background",pid2);
		}
	}
	close(fds[0]);
	close(fds[1]);
}

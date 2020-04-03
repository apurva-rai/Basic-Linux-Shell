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


struct Job {
    int pid;
    int id;
    char* cmd;
};
static int job_count = 0;
static Job jobs[100];
static char* env;
static char* dir;



void display_jobs()
{
    int i;
    printf("\nActive jobs:\n");
    printf("| %7s  | %7s | %7s |\n", "Job ID", "PID  ", "Command");
    for (i=0; i < job_count; i++)
	{
        if (kill(jobs[i].pid, 0) == 0)
		{
            printf("|  [%7d] | %7d | %7s |\n", jobs[i].id, jobs[i].pid, jobs[i].cmd);
        }
    }
}


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
//     int type;
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
//         waitpid(pid, &type, 0);
//         if (type == 1)
// 		{
//             fprintf(stderr, "%s\n", "Poo poo\n");
//         }
//     }
// }



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

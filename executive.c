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
#define BSIZE 256
#define true 1
#define false 0

using namespace std;




void executive(char ***argv, int background, char **env)
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


void execute(char** str)
{
    int status;
    pid_t pid;
    pid = fork();
    if (pid == 0)
	{
        if (strlen(str[0]) > 0)
		{
            if (execvp(str[0], str) < 0)
			{
                fprintf(stderr, "Invalid command\n");
                exit(0);
            }
        }
		else
		{
            if (execvp(str[0], NULL) < 0)
			{
                fprintf(stderr, "Invalid command\n");
                exit(0);
            }
        }
    }
    else
	{
        waitpid(pid, &status, 0);
        if (status == 1)
		{
            fprintf(stderr, "%s\n", "Poo poo\n");
        }
    }
}

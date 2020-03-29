#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <string>
#include <errno.h>
#define BSIZE 256

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

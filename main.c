//Project: Quite a Shell (Quash)
//Made by Apurva Rai and Meet Kapadia

#include "Parse.c"
#include "executive.c"

int fd;

struct Job {
	pid_t pid;
	int id;
	char cmd [300];
};

static struct Job jobs[64];
int job_count = 0;





void menu()
{
    printf("\e[1;1H\e[2J"); //Clear console
    printf("\t\tQuite a Shell\n\t\tMade by Apurva Rai and Meet Kapadia\n\t\tSee ReadMe for instructions\n");

}

/* Handles reading input from prompt
 */
char* firstParse(char* line, int& inFile)
{
	inFile = read(fd, line, BYTESIZE);
	if (inFile <= 0)
	{
		close(fd);
		read(0, line, BYTESIZE);
	}
	return line;
}

int main (int argc, char **argv, char **envp)

{

    menu();
    commandSignal* c1;
	commandSignal* c2;
    char *buff;
	char **env;
    char line[BYTESIZE];
	char temp[BYTESIZE];
	int inFile = 0;
	char* name;
	char* input;
	char* output;
    char* pipeU;

	while (1)
	{
		printf("%s> ",getcwd(NULL,0));
		fflush(stdout);
		memset(line, '\0', sizeof(line));
		buff = firstParse(line, inFile);
		pipeU = strpbrk(buff, "|");
		input = strpbrk(buff, "<");
		output = strpbrk(buff, ">");
		// printf("pipe        :%s\n",pipeU);
		// printf("in from file:%s\n", input);
		// printf("out to file :%s\n", output);
        printf("%s",buff);
		if ((strncmp(buff,"exit",4)==0) || (strncmp(buff,"quit",4)==0))
        {
			break;
		// cd
		}
        else if (strncmp(buff, "cd", 2) == 0)
        {
			cd(buff);
			continue;
		// pwd
		}
        else if (strncmp(buff, "pwd",3)==0)
        {
			printf("%s\n",getcwd(NULL, 0));
			continue;
		// $PATH
		}
        else if (strcmp(buff, "$PATH\n")==0)
        {
			printf("\nPATH: %s\n\n", getenv("PATH"));
			continue;
		// set PATH
		}
        else if (strncmp(buff, "set PATH=",9)==0)
        {
			// skip the "set PATH="
			buff+=9;
			setenv("PATH",buff,1);
			printf("PATH was set to %s\n", getenv("PATH"));
			continue;
		// $HOME
		}
        else if (strcmp(buff, "$HOME\n")==0)
        {
			printf("\nHOME: %s\n\n", getenv("HOME"));
			continue;
		// set HOME
		}
        else if (strncmp(buff, "set HOME=",9)==0)
        {
			// skip the "set HOME="
			buff+=9;
			setenv("HOME",buff,1);
			printf("HOME was set to %s\n", getenv("HOME"));
			continue;
		// clear
		}
        else if (strcmp(buff, "clear\n")==0)
        {
			printf("\e[1;1H\e[2J");
			continue;
		// jobs
		}
        else if (strncmp(buff, "jobs",4)==0)
        {
            printf("\nCurrent Jobs:\n");
		    printf("Job ID, PID, Command\n\n");
		    for(int i=0; i < job_count; i++)
            {
			             if(waitpid(jobs[i].pid, NULL, WNOHANG) == 0 || (kill(jobs[i].pid, 0) == 0))
                         {
				                         printf("[%d] %d || %s\n\n", jobs[i].id, jobs[i].pid, jobs[i].cmd);
			             }
		    }
		    printf("\n");
			continue;
		}

	/*----------
		RUNNING
		EXECUTABLES
	-----------*/

        else
        {
			system(buff);
			#if 0
			if ((output == NULL) && (pipeU == NULL) && (input == NULL))
            {
				// standard command
				printf("<standard command>\n");
				c1 = parse(buff, env);
				executive1(&((*c1).argv), (*c1).run_in_background, (*c1).env, buff);
				// Reset variables
				memset(c1, 0, sizeof(*c1));
				memset(buff, '\0', sizeof(buff));
				free(c1);
				c1 = NULL;
				printf("\n");
				continue;
			}
			if ((output == NULL) && (pipeU != NULL) && (input == NULL))
            {
				// pipe only
				printf("<pipe only>\n");
				char* token = strtok(buff, "|\n");
				char firstStr[BYTESIZE];
				strcpy(firstStr,token);
				token = strtok(buff, "|\n");
				char secondStr[BYTESIZE];
				strcpy(secondStr,token);
				c1 = parse(firstStr, env);
				c2 = parse(secondStr, env);
				pipeFunc(&((*c1).argv), (*c1).run_in_background, (*c1).env, &((*c2).argv), (*c2).run_in_background, (*c2).env);
				free(c1);
				free(c2);
				c1 = NULL;
				c2 = NULL;
				memset(firstStr, '\0', sizeof(char) * BYTESIZE);
				memset(secondStr, '\0', sizeof(char) * BYTESIZE);
				memset(buff, '\0', sizeof(char) * BYTESIZE);


				continue;
			}
			if ((output != NULL) && (pipeU == NULL) && (input == NULL))
            {
				// output redirection only
				printf("<output redirection only>\n");
				// Tokenize to remove '>' and '\n', getting first the command
				char *token = strtok(buff, ">\n");
				char firstStr[256];
				strcpy(firstStr, token);
				printf("first string  :%s\n",firstStr);
				c1 = parse(firstStr, env);
				// Advance token to get input file
				char secondStr[256];
				token = strtok(NULL, "<\n");
				if (token!=NULL)
                {
					strcpy(secondStr, token);
					printf("second string:%s\n",secondStr);
				}

				// Execute the command and output the result to a file
				toFile(&((*c1).argv), (*c1).run_in_background, (*c1).env, secondStr);
				// Reset variables
				memset(firstStr, '\0', sizeof(char) * BYTESIZE);
				memset(secondStr, '\0', sizeof(char) * BYTESIZE);
				memset(buff, '\0', sizeof(buff));
				free(c1);
				c1 = NULL;

				continue;
			}
			if ((output != NULL) && (pipeU != NULL) && (input == NULL))
            {
				// output redirection and pipe
				printf("<output redirection and pipe>\n");
				continue;
			}
			if (input != NULL)
            {
				// input redirection, read input from file
				printf("input redirection, read input from file\n");
				strncpy(name, buff+3, strlen(buff)-1);
				fd = open(name, O_RDONLY);
				#if 0
				// Tokenize to remove '<' and '\n', getting first the command
				char *token = strtok(buff, "<\n");
				char firstStr[BYTESIZE];
				strcpy(firstStr, token);
				// Advance token to get input file
				char secondStr[BYTESIZE];
				token = strtok(NULL, "<\n");
				if (token!=NULL)
                {
					strcpy(secondStr, token);
				}

				// parse buffs
				c1 = parse(firstStr, env);
				// Execute the command and output the result to a file
				inFile(&((*c1).argv), (*c1).run_in_background, (*c1).env, secondStr);

				// Reset variables
				memset(firstStr, '\0', sizeof(char) * BYTESIZE);
				memset(secondStr, '\0', sizeof(char) * BYTESIZE);
				memset(buff, '\0', sizeof(buff));
				free(c1);
				c1 = NULL;

				#endif
				continue;
			}

		#endif
		}
	}
	return 0;
}

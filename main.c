//Project: Quite a Shell (Quash)
//Made by Apurva Rai and Meet Kapadia

#include "Parse.c"
#include "executive.c"


void menu()
{
    printf("\e[1;1H\e[2J"); //Clear console
    printf("\t\tQuite a Shell\n\t\tMade by Apurva Rai and Meet Kapadia\n\t\tSee ReadMe for instructions\n");

}


int main (int argc, char **argv, char **envp)

{

    menu();

  char line[BSIZE];
	char *buffer;
	char **env;
	char temp[BSIZE];
	int fromfile = 0;
	char* filename;

	command* commandOne;
	command* commandTwo;

	// string after |, <, or >
	char* pipe_useage;
	char* file_input;
	char* file_output;

	while (1)
	{	// display QUASH prompt and get user input.


		printf("%s> ",getcwd(NULL,0));
		fflush(stdout);
		// ensure line is empty
		memset(line, '\0', sizeof(line));
		// read in line
		buffer = accept(line, fromfile);

		/* look for first occurrence of |, <, or > and
		 * report on trailing string
		 */
		pipe_useage = strpbrk(buffer, "|");
		file_input = strpbrk(buffer, "<");
		file_output = strpbrk(buffer, ">");
		printf("pipe        :%s\n",pipe_useage);
		printf("in from file:%s\n", file_input);
		printf("out to file :%s\n", file_output);


		/* Figure out what commands to execute
		 */
		 // exit/quit
		if ((strncmp(buffer,"exit",4)==0) || (strncmp(buffer,"quit",4)==0)){
			break;
		// cd
		}else if (strncmp(buffer, "cd", 2) == 0) {
			cd(buffer);
			continue;
		// pwd
		} else if (strncmp(buffer, "pwd",3)==0) {
			printf("%s\n",getcwd(NULL, 0));
			continue;
		// $PATH
		} else if (strcmp(buffer, "$PATH\n")==0){
			printf("\nPATH:              %s\n\n", getenv("PATH"));
			continue;
		// set PATH
		} else if (strncmp(buffer, "set PATH=",9)==0){
			// skip the "set PATH="
			buffer+=9;
			setenv("PATH",buffer,1);
			printf("PATH set to %s\n", getenv("PATH"));
			continue;
		// $HOME
		} else if (strcmp(buffer, "$HOME\n")==0){
			printf("\nHOME:              %s\n\n", getenv("HOME"));
			continue;
		// set HOME
		} else if (strncmp(buffer, "set HOME=",9)==0){
			// skip the "set HOME="
			buffer+=9;
			setenv("HOME",buffer,1);
			printf("HOME set to %s\n", getenv("HOME"));
			continue;
		// clear
		} else if (strcmp(buffer, "clear\n")==0) {
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
			continue;
		// jobs
		} else if (strncmp(buffer, "jobs", 4)==0){


			printf("THIS SPACE INTENTIONALLY LEFT BLANK\n");


			continue;
		} else {
			system(buffer);
			#if 0
			if ((file_output == NULL) && (pipe_useage == NULL) && (file_input == NULL)){
				// standard command
				printf("<standard command>\n");

				commandOne = parse_raw_input(buffer, env);
				execute(&((*commandOne).argv), (*commandOne).run_in_background, (*commandOne).env);
				// Reset variables
				memset(commandOne, 0, sizeof(*commandOne));
				memset(buffer, '\0', sizeof(buffer));
				free(commandOne);
				commandOne = NULL;
				printf("\n");

				continue;
			}
			if ((file_output == NULL) && (pipe_useage != NULL) && (file_input == NULL)){
				// pipe only
				printf("<pipe only>\n");

				char* token = strtok(buffer, "|\n");
				char firstStr[BSIZE];
				strcpy(firstStr,token);
				token = strtok(buffer, "|\n");
				char secondStr[BSIZE];
				strcpy(secondStr,token);
				commandOne = parse_raw_input(firstStr, env);
				commandTwo = parse_raw_input(secondStr, env);

				execute_pipe(&((*commandOne).argv), (*commandOne).run_in_background, (*commandOne).env, &((*commandTwo).argv), (*commandTwo).run_in_background, (*commandTwo).env);

				free(commandOne);
				free(commandTwo);
				commandOne = NULL;
				commandTwo = NULL;
				memset(firstStr, '\0', sizeof(char) * BSIZE);
				memset(secondStr, '\0', sizeof(char) * BSIZE);
				memset(buffer, '\0', sizeof(char) * BSIZE);


				continue;
			}
			if ((file_output != NULL) && (pipe_useage == NULL) && (file_input == NULL)){
				// output redirection only
				printf("<output redirection only>\n");
				// Tokenize to remove '>' and '\n', getting first the command
				char *token = strtok(buffer, ">\n");
				char firstStr[256];
				strcpy(firstStr, token);
				printf("first string  :%s\n",firstStr);
				commandOne = parse_raw_input(firstStr, env);
				// Advance token to get input file
				char secondStr[256];
				token = strtok(NULL, "<\n");
				if (token!=NULL){
					strcpy(secondStr, token);
					printf("second string:%s\n",secondStr);
				}

				// Execute the command and output the result to a file
				execute_to_file(&((*commandOne).argv), (*commandOne).run_in_background, (*commandOne).env, secondStr);

				// Reset variables
				memset(firstStr, '\0', sizeof(char) * BSIZE);
				memset(secondStr, '\0', sizeof(char) * BSIZE);
				memset(buffer, '\0', sizeof(buffer));
				free(commandOne);
				commandOne = NULL;

				continue;
			}
			if ((file_output != NULL) && (pipe_useage != NULL) && (file_input == NULL)){
				// output redirection and pipe
				printf("<output redirection and pipe>\n");

				continue;
			}
			if (file_input != NULL) {
				// input redirection, read input from file
				printf("input redirection, read input from file\n");
				strncpy(filename, buffer+3, strlen(buffer)-1);
				fd = open(filename, O_RDONLY);
				#if 0
				// Tokenize to remove '<' and '\n', getting first the command
				char *token = strtok(buffer, "<\n");
				char firstStr[BSIZE];
				strcpy(firstStr, token);
				// Advance token to get input file
				char secondStr[BSIZE];
				token = strtok(NULL, "<\n");
				if (token!=NULL){
					strcpy(secondStr, token);
				}

				// parse buffers
				commandOne = parse_raw_input(firstStr, env);
				// Execute the command and output the result to a file
				execute_from_file(&((*commandOne).argv), (*commandOne).run_in_background, (*commandOne).env, secondStr);

				// Reset variables
				memset(firstStr, '\0', sizeof(char) * BSIZE);
				memset(secondStr, '\0', sizeof(char) * BSIZE);
				memset(buffer, '\0', sizeof(buffer));
				free(commandOne);
				commandOne = NULL;

				#endif
				continue;
			}

		#endif
		}
	}
	return 0;
}

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
    char tempBuffer[BYTESIZE];
    char** environment = NULL;

    menu();

    fgets(tempBuffer, BYTESIZE, stdin);

    printf("%s",tempBuffer);
    parse_raw_input(tempBuffer,environment);
    return 0;
}

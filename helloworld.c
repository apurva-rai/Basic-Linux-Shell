#include <stdio.h>     /* standard I/O functions                         */
#include <stdlib.h>    /* exit                                           */
#include <stdbool.h>   /* using bool data type			                     */
#include <unistd.h>    /* standard unix functions, like getpid()         */
#include <signal.h>    /* signal name macros, and the signal() prototype */
#include <sys/types.h> /* needed for pid_t */
#include <sys/utsname.h> /* needed for system information */
#include <dirent.h>		/* for ls */
#include <string.h>    /* needed for mitch reasons */
#include <fcntl.h>     /* needed for output redirection */
#include <wait.h>

int main() {
    printf("Hello World\n");
}

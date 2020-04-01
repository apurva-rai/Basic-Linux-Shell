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
#define BSIZE 256
#define true 1
#define false 0

using namespace std;


struct Jobs
{
    int pid;
    int id;
    char* cmd;
};
static int job_count = 0;
static struct Jobs jobs[100];
static char* env;
static char* dir;

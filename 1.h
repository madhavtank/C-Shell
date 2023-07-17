#ifndef new
#define new

#include<stdio.h>  
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<pwd.h>
#include<dirent.h>
#include<time.h>
#include<grp.h>
#include<sys/types.h>
#include<sys/time.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<unistd.h>
#include<errno.h>
#include<fcntl.h>
#include<termios.h>
#include<ctype.h>

typedef struct Process{
    int index;
    int run;
    char name[1000];
    pid_t pid;
}Process;

char tilda[2000],*bg_command[500];
int bg_pid[500];
int current_id;
Process all_proc[1000];
int bg_index,singal_match;

#endif
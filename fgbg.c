#include "1.h"
#include "functions.h"

extern long int time_took;

int flag=0;
void add_bg(int pid,char *command)
{
    if(pid==0) return;
    int pos=0;
    while(bg_pid[pos]!=0) pos++;
    bg_pid[pos]=pid;
    bg_command[pos] = (char *)malloc(1000);
    strcpy(bg_command[pos], command);

    all_proc[bg_index].index=bg_index;
    all_proc[bg_index].run=1;
    strcpy(all_proc[bg_index].name,command);
    all_proc[bg_index].pid=pid;
}

void bgcall(int k,char *parts[])
{
    pid_t pid=fork();
    add_bg(pid,parts[0]);
    if(pid!=0)
    printf("[%d] %d\n",bg_index,pid);
    for(int i=1;i<k;i++)
    {
        strcat(all_proc[bg_index].name," ");
        strcat(all_proc[bg_index].name,parts[i]);
    }
    bg_index++;
    int status;
    if(pid==0)
    {
        setpgrp();
        parts[k]=NULL;
        int y=execvp(parts[0],parts);
        exit(0);
    }
    else if(pid==-1)
    perror("error\n");
}

void fgcall(int k,char *parts[])
{
    flag=1;
    time_t begin=time(NULL),end;
    pid_t pid=fork();
    int status;
    if(pid==0)
    {
        // signal(SIGINT,SIG_DFL);
        // signal(SIGTSTP,SIG_DFL);

        setpgid(0, 0);
        parts[k]=NULL;
        int y=execvp(parts[0],parts);
        printf("Command '%s' not found\n",parts[0]);
        flag=0;
    }
    else if(pid==-1)
    perror("error\n");
    else
    {
        signal(SIGTTIN,SIG_IGN);
        signal(SIGTTOU,SIG_IGN);
        tcsetpgrp(STDIN_FILENO,pid);

        waitpid(pid,&status,WUNTRACED | WCONTINUED);
        end=time(NULL);

        tcsetpgrp(STDIN_FILENO,getpgrp());
        signal(SIGTTIN,SIG_DFL);
        signal(SIGTTOU,SIG_DFL);
        
        if(WIFSTOPPED(status))
        {
            printf("process with pid %d stopped\n",pid);
            all_proc[bg_index].index=bg_index;
            all_proc[bg_index].run=-1;
            all_proc[bg_index].pid=pid;
            strcpy(all_proc[bg_index].name,parts[0]);
            for(int i=1;i<k;i++)
            {
                strcat(all_proc[bg_index].name," ");
                strcat(all_proc[bg_index].name,parts[i]);
            }
            bg_index++;
        }
    }
    if(flag)
    time_took=end-begin;
    else
    time_took=-1;
}

void fgbg(int k,char *parts[])
{
    char *temp[500];
    int ptr=0;
    for(int i=0;i<k;i++)
    {
        if(strcmp("&",parts[i])==0)
        {
            if(ptr>0)
            {
                bgcall(ptr,temp);
                ptr=0;
            }
        }
        else
        {
            temp[ptr++]=(char *)malloc(1000);
            strcpy(temp[ptr-1],parts[i]);
        }
    }
    if(ptr!=0)
    fgcall(ptr,temp);
}
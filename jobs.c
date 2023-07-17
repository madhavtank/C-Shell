#include "1.h"
#include "functions.h"

void jobs(char command[])
{
    int r=0,s=0;
    if(strstr(command,"-r")!=NULL) r=1;
    if(strstr(command,"-s")!=NULL) s=1;

    if(strstr(command,"-r")==NULL && strstr(command,"-s")==NULL) r=s=1;

    for(int i=1;i<bg_index;i++)
    {
        for(int j=1;j<bg_index-1;j++)
        {
            if(strcmp(all_proc[j].name,all_proc[j+1].name)>0)
            {
                char *temp;
                temp=malloc(1000);
                strcpy(temp,all_proc[j].name);
                strcpy(all_proc[j].name,all_proc[j+1].name);
                strcpy(all_proc[j+1].name,temp);
                free(temp);

                int temp2;
                temp2=all_proc[j].index;
                all_proc[j].index=all_proc[j+1].index;
                all_proc[j+1].index=temp2;

                int temp3=all_proc[j].pid;
                all_proc[j].pid=all_proc[j+1].pid;
                all_proc[j+1].pid=temp3;

                temp3=all_proc[j].run;
                all_proc[j].run=all_proc[j+1].run;
                all_proc[j+1].run=temp3;
            }
        }
    }

    for(int i=1;i<bg_index;i++)
    {
        if(all_proc[i].run==-1 && s)
        {
            printf("[%d]   Stopped\t\t\t%s [%d]\n",all_proc[i].index,all_proc[i].name,all_proc[i].pid);
        }
        else if(all_proc[i].run==1 && r)
        {
            printf("[%d]   Running\t\t\t%s [%d]\n",all_proc[i].index,all_proc[i].name,all_proc[i].pid);
        }
    }
}

void sig(char command[])
{
    char *parts[1000];
    parts[0]=strtok(command," ""\t");
    int k=0;
    while(parts[k]!=NULL)
    parts[++k]=strtok(NULL," ""\t");

    if(k!=3)
    printf("error\n");
    else
    {
        int ind=atoi(parts[1]),sig=atoi(parts[2]);
        if(ind>=bg_index)
        {
            printf("no job with given number exists\n");
        }
        else
        {
            for(int i=1;i<=bg_index;i++)
            {
                if(all_proc[i].index==ind)
                {
                    kill(all_proc[i].pid,sig);
                    return;
                }
            }
        }
    }
}

void fg(char command[])
{
    char *parts[1000];
    parts[0]=strtok(command," ""\t");
    int k=0;
    while(parts[k]!=NULL)
    parts[++k]=strtok(NULL," ""\t");

    if(k!=2)
    printf("error\n");
    else
    {
        int ind=atoi(parts[1]);
        if(ind>=bg_index)
        {
            printf("no job with given number exists\n");
        }
        else
        {
            for(int i=1;i<=bg_index;i++)
            {
                if(all_proc[i].index==ind)
                {
                    if(all_proc[i].run==0) return;
                    signal(SIGTTIN,SIG_IGN);
                    signal(SIGTTOU,SIG_IGN);
                    tcsetpgrp(STDIN_FILENO,all_proc[i].pid);
                    kill(all_proc[i].pid,SIGCONT);
                    if(all_proc[i].run==1)
                    {
                        int pos=0;
                        while(pos<500 && bg_pid[pos]!=all_proc[i].pid) pos++;
                        if(pos<500)
                        {
                            bg_pid[pos]=0;
                            free(bg_command[pos]);
                        }
                    }
                    all_proc[i].run=0;
                    waitpid(-1,NULL,WUNTRACED);
                    tcsetpgrp(STDIN_FILENO,getpgrp());
                    signal(SIGTTIN,SIG_DFL);
                    signal(SIGTTOU,SIG_DFL);
                    return;
                }
            }
        }
    }
}

void bg(char command[])
{
    char *parts[1000];
    parts[0]=strtok(command," ""\t");
    int k=0;
    while(parts[k]!=NULL)
    parts[++k]=strtok(NULL," ""\t");

    if(k!=2)
    printf("error\n");
    else
    {
        int ind=atoi(parts[1]);
        if(ind>=bg_index)
        {
            printf("no job with given number exists\n");
        }
        else
        {
            for(int i=1;i<=bg_index;i++)
            {
                if(all_proc[i].index==ind)
                {
                    if(all_proc[i].run==1) return;
                    all_proc[i].run=0;
                    kill(all_proc[i].pid,SIGTTIN);
                    kill(all_proc[i].pid,SIGCONT);
                    char *tokenize[1000];
                    tokenize[0]=strtok(all_proc[i].name," ""\t");
                    int r=0;
                    while(tokenize[r]!=NULL)
                    tokenize[++r]=strtok(NULL," ""\t");
                    bgcall(r,tokenize);
                    return;
                }
            }
        }
    }    
}
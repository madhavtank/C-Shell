#include "1.h"
#include "functions.h"

int pipe_check(char command[])
{
    int len=strlen(command),val=0;
    for(int i=0;i<len;i++)
    {
        if(command[i]=='|')
        val=1;
    }
    return val;
}

void handle_pipe(char command[])
{
    char *token[1000];
    int k=0;
    token[k]=strtok(command,"|");

    while(token[k]!=NULL)
    {
        token[++k]=strtok(NULL,"|");
    }
    
    for(int i=0;i<k;i++)
    {
        char command_cpy[1000];
        strcpy(command_cpy,token[i]);

        char *parts[1000];
        int l=0;
        parts[0]=strtok(command_cpy," ""\t");
        while(parts[l]!=NULL)
        {
            parts[++l]=strtok(NULL," ""\t");
        }

        int pipe1[2],pipe2[2];

        if(i&1)
        {
            pipe(pipe2);
        }
        else
        {
            pipe(pipe1);
        }

        pid_t pid=fork();
        if(pid==0)
        {
            if(i==k-1)
            {
                if(i&1)
                {
                    dup2(pipe1[0],0);
                }
                else
                {
                    dup2(pipe2[0],0);
                }
            }
            else if(i&1)
            {
                dup2(pipe1[0],0);
                close(pipe1[1]);
                dup2(pipe2[1],1);
            }
            else
            {
                if(i!=0)
                {
                dup2(pipe2[0],0);
                close(pipe1[0]);
                dup2(pipe1[1],1);
                }
                else
                {
                    dup2(pipe1[1],1);
                    close(pipe1[0]);
                }
            }

            int len=strlen(token[i]),red_check=0;
            for(int j=0;j<len;j++)
            {
                if(token[i][j]=='<' || token[i][j]=='>')
                red_check=1;
            }
            char topass[1000];
            strcpy(topass,token[i]);

            if(red_check==1)
            redirection(topass);
            else
            {
                int y=execvp(parts[0],parts);
                if(y<0)
                perror("Error:Command not found\n");
            }
            exit(0);
        }
        else
        {
            wait(NULL);
            if(i==k-1)
            {
                if(i&1)
                close(pipe1[0]);
                else
                close(pipe2[0]);
            }
            else if(i&1)
            {
                close(pipe1[0]);
                close(pipe2[1]);
            }
            else
            {
                if(i!=0)
                {
                close(pipe2[0]);
                close(pipe1[1]);
                }
                else
                close(pipe1[1]);
            }
        }
    }
}
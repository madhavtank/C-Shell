#include "functions.h"
#include "1.h"

int status,file_des,file_des2;

void execut(char *temp2[],int k,int old_in,int old_out)
{   
    int y=execvp(temp2[0],temp2);
    if(y<0)
    {
        perror("error\n");
        dup2(old_out,1);
        close(file_des);
    }
}

void redirection(char command[])
{

    int append=0,out=0,in=0;
    if(strstr(command,">>")==NULL)
    append=0;
    else
    append=1;

    char input_file[10000],output_file[10000],sep[10];

    if(append==1)
    strcpy(sep,">>");
    else
    strcpy(sep,">");

    char *parts[10000];

    parts[0]=strtok(command,sep);
    int k=0;
    while(parts[k]!=NULL)
    {
        k++;
        parts[k]=strtok(NULL,sep);
    }

    if(k==2)
    {
        out=1;
        char temp[10000],*inp;
        inp=malloc(10000);
        strcpy(temp,parts[1]);
        inp=strtok(temp," \t\r\n");
        strcat(inp,"\0");
        for(int i=0;inp[i]!='\0';i++) output_file[i]=inp[i];
        output_file[strlen(inp)]='\0';
    }

    k=0;
    char *temp[10000];
    temp[0]=strtok(parts[0],"<");
    while(temp[k]!=NULL)
    {
        temp[++k]=strtok(NULL,"<");
    }

    if(k==2)
    {
        in=1;
        char *inp;
        inp=(char *)malloc(10000);
        strcpy(input_file,temp[1]);
        inp=strtok(input_file," \r\n\t");
        strcat(inp,"\0");
        for(int i=0;inp[i]!='\0';i++) input_file[i]=inp[i];
        input_file[strlen(inp)]='\0';
    }

    k=0;
    char *temp2[10000];
    temp2[0]=strtok(temp[0]," ""\t");
    while(temp2[k]!=NULL)
    {
        temp2[++k]=strtok(NULL," ""\t");
    }

    if(in==1 && out==0)
    {
        int old_in=dup(STDIN_FILENO);
        int pid=fork();
        file_des=open(input_file,O_RDONLY);
        if(file_des<0) printf("File doesn't exist\n");
        if(pid==0)
        {
            if(file_des>=0)
            {
                dup2(file_des,0);
                execut(temp2,k,old_in,-1);
                dup2(old_in,0);
            }
            exit(0);
        }
        else
        while (wait(&status) != pid) {}
    }
    if(in==0 && out==1)
    {
        int old_out = dup(STDOUT_FILENO);
        int pid = fork();

        if(append==0)
        file_des = open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else if(append==1)
        file_des = open(output_file, O_APPEND | O_WRONLY | O_CREAT, 0644);
        dup2(file_des,1);

        if (pid==0)
        {
            execut(temp2,k,-1,old_out);
            exit(0);
        }
        else
        {
            while (wait(&status) != pid) {}
            dup2(old_out, 1);
            close(file_des);
        }
    }
    if(in && out)
    {
        int old_in=dup(STDIN_FILENO),old_out=dup(STDOUT_FILENO);
        int pid = fork();
        file_des=open(input_file,O_RDONLY);

        if(append==0)
        file_des2=open(output_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else if(append == 1)
        file_des2=open(output_file, O_APPEND | O_WRONLY | O_CREAT, 0644);

        dup2(file_des2,1);
        if(file_des<0) printf("File doesn't exist\n");
        
        if(pid==0)
        {
            if(file_des>=0)
            {
                dup2(file_des,0);
                execut(temp2,k,old_in,old_out);
                dup2(old_in,0);
            }
            exit(0);
        }
        else
        {
            while(wait(&status)!=pid) {}
            dup2(old_out,1);
            close(file_des);
        }
    }
}
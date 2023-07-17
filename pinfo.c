// #include "pinfo.h"

#include "1.h"
#include "functions.h"

void pinfo(int k,char *parts[])
{
    if(k>1)
    {
        perror("error");
        return;
    }
    pid_t pid;
    if(k==0) pid=getpid();
    else pid=atoi(parts[0]);
    char filename[10000];
    sprintf(filename,"/proc/%d/stat",pid);
    int f=open(filename,O_RDONLY);
    if(f<0) perror("No such process Exists");
    else
    {
        FILE *fileptr;
        fileptr=fopen(filename,"r");
        char temp[1000][100],status[20],memory[20];
        int i=0;
        while(fscanf(fileptr,"%[^ ] ",temp[i])!=EOF) i++;
        strcpy(status,temp[2]);
        strcpy(memory,temp[22]);
        if(strcmp(temp[4],temp[7])==0)
        strcat(status,"+");
        filename[0]='\0';
        sprintf(filename,"/proc/%d/exe", pid);
        char buff[10000];
        int x=readlink(filename,buff,sizeof(buff));
        buff[x]='\0';
        printf("pid : %d\n",pid);
        printf("process status : {%s}\n",status);
        printf("memory : %s {Virtual Memory}\n",memory);
        char final_path[10000];
        if(strcmp(buff,tilda)==0)
        final_path[0]='~';
        else
        {
            if(strstr(buff,tilda)==NULL)
            {
                strcpy(final_path,buff);
            }
            else
            {
                int len=strlen(tilda),ptr=1;
                final_path[0]='~';
                for(int i=len;i<strlen(buff);i++)
                {
                    final_path[ptr++]=buff[i];
                }
            }
        }
        printf("executable path : %s\n",final_path);
    }
}
#include "1.h"
#include "functions.h"

int min(int a,int b)
{
    if(a<b) return a;
    return b;
}

char * ac(char *inp,char *space,int *no_match)
{

    *no_match=1;
    singal_match=0;
    memset(toreturn,'\0',100);
    if(strcmp(space,"")==0)
    {
        int ok=1;
        for(int i=0;i<strlen(inp);i++)
        {
            if(inp[i]==' ')
            ok=0;
        }
        if(ok)
        {
            strcpy(space,inp);
        }
    }
    char all[100][100];
    DIR *dh=opendir(".");
    struct dirent *d;
    d=readdir(dh);
    int ptr=0;
    while(d!=NULL)
    {
        if(d->d_name[0]=='.'){d=readdir(dh); continue; }
        strcpy(all[++ptr],d->d_name);
        d=readdir(dh);
    }

    for(int i=0;i<ptr;i++)
    {
        struct stat info;
        char complete_filename[1000];
        strcpy(complete_filename,"./");
        strcat(complete_filename,all[i]);
        stat(complete_filename,&info);
        if(S_ISDIR(info.st_mode))
        {
            strcat(all[i],"/");
        }
    }

    if(strcmp(inp,"")==0)
    {
        for(int i=0;i<ptr;i++)
        printf("%s\n",all[i]);
        return NULL;
    }

    char common[100][100];
    int ptr2=0;
    int minlen=1000000;
    int len=strlen(space);
    for(int i=0;i<ptr;i++)
    {
        if(len>strlen(all[i])) continue;
        int ok=1;
        for(int j=0;j<len;j++)
        {
            if(space[j]!=all[i][j])
            {
                ok=0;
            }
        }
        if(ok)
        {
            minlen=min(minlen,strlen(all[i]));
            strcpy(common[ptr2++],all[i]);
        }
    }

    if(ptr2==0)
    {
        *no_match=0;
        return NULL;
    }
    else if(ptr2==1)
    {
        singal_match=1;
        int ptr3=0;
        for(int i=len;i<strlen(common[0]);i++)
        toreturn[ptr3++]=common[0][i];

        if(toreturn[ptr3-1]!='/')
        strcat(toreturn," ");
    }
    else
    {
        char lcp[100];
        memset(lcp,'\0',100);
        int r=0;
        for(int j=len;j<minlen;j++)
        {
            char a=common[0][j];
            int ok=1;
            for(int i=0;i<ptr2;i++)
            {
                if(common[i][j]!=a)
                ok=0;
            }
            if(ok)
            lcp[r++]=a;
            else
            break;
        }
        strcpy(toreturn,lcp);
        printf("\n");
        for(int i=0;i<ptr2;i++)
        {
            printf("%s\n",common[i]);
        }
    }
}
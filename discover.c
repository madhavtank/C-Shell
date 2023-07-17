// #include "discover.h"

#include "1.h"
#include "functions.h"

void dfs(char *target,char *file,int d,int f)
{
    if(file==NULL)
    {
        if((d==0 && f==0) || d)
        printf("%s\n",target);
        DIR *dh=opendir(target);
        struct dirent *d1=readdir(dh);
        while(d1!=NULL)
        {
            if(d1->d_name[0]=='.')
            {
                d1=readdir(dh);
                continue;
            }
            char fullname[5000];
            strcpy(fullname,target);
            strcat(fullname,"/");
            strcat(fullname,d1->d_name);
            struct stat info;
            stat(fullname,&info);
            if(S_ISDIR(info.st_mode))
            dfs(fullname,NULL,d,f);
            else
            {
                if((d==0 && f==0)||f)
                printf("%s\n",fullname);
            }
            d1=readdir(dh);
        }
    }
    else
    {
        DIR *dh=opendir(target);
        struct dirent *d1=readdir(dh);
        while(d1!=NULL)
        {
            if(d1->d_name[0]=='.')
            {
                d1=readdir(dh);
                continue;
            }
            char fullname[1000];
            strcpy(fullname,target);
            strcat(fullname,"/");
            strcat(fullname,d1->d_name);
            struct stat info;
            stat(fullname,&info);
            if(strcmp(d1->d_name,file)==0)
            {
                // printf("%s\n",fullname);
                if(S_ISDIR(info.st_mode))
                {
                    if((d==0 && f==0)||d)
                    {
                        printf("%s\n",fullname);
                        dfs(fullname,file,d,f);
                    }
                }
                else
                {
                    if((d==0 && f==0)||f)
                    printf("%s\n",fullname);
                }
            }
            else
            {
                if(S_ISDIR(info.st_mode))
                dfs(fullname,file,d,f);
            }
            d1=readdir(dh);
        }
    }
}
void discover(int k,char *parts[])
{
    int fl=0,td=0,d=0,f=0,invalid=0;
    char file[10000],target[10000];
    for(int i=0;i<k;i++)
    {
        if(strcmp(parts[i],"-d")==0) d=1;
        else if(strcmp(parts[i],"-f")==0) f=1;
        else if(parts[i][0]=='"' && parts[i][strlen(parts[i])-1]=='"')
        {
            fl=1;
            strcpy(file,parts[i]);
        }
        else
        {
            char fullname[10000];
            getcwd(fullname,10000);
            strcat(fullname,"/");
            strcat(fullname,parts[i]);
            struct stat info;
            if(stat(fullname,&info)==0 && S_ISDIR(info.st_mode))
            {
                td=1;
                strcpy(target,parts[i]);
            }
            else
            {
                invalid=1;
            }
        }
    }
    if(fl)
    {
        int len=strlen(file);
        for(int i=1;i<len;i++) file[i-1]=file[i];
        file[len-2]=file[len-1]='\0';
        if(td)
        dfs(target,file,d,f);
        else
        {
            if(invalid==0)
            dfs(".",file,d,f);
        }
    }
    else
    {
        if(td)
        dfs(target,NULL,d,f);
        else
        {
            if(invalid==0)
            dfs(".",NULL,d,f);
        }
    }
}
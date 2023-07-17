// #include"headers.h"

#include "1.h"
#include "functions.h"

#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
#define GREEN "\x1b[32m"
int cmp(const void *p1, const  void *p2)
{
    return strcmp(* (char **) p1, * (char **) p2)>=0;
}
int curr_size=0;
char dirs[1000][1000],files[1000][100],dne[1000][100];
void taamjaam()
{
    curr_size=0;
    memset(dirs,'\0',sizeof(dirs));
    memset(files,'\0',sizeof(files));
    memset(dne,'\0',sizeof(dne));
}
void print_lsl(char *directory,char *filename)
{
    char complete_filename[1000];
    strcpy(complete_filename,directory);
    strcat(complete_filename,"/");
    strcat(complete_filename,filename);
    struct stat info;
    stat(complete_filename,&info);
    struct passwd *password=getpwuid(info.st_uid);
    struct group *gp=getgrgid(info.st_gid);
    char time[100];
    strftime(time,100,"%b %d %H:%M",localtime(&info.st_mtime));
    if(S_ISDIR(info.st_mode))printf("d");
    else printf("-");
    (info.st_mode & S_IRUSR)?printf("r"):printf("-");
    (info.st_mode & S_IWUSR)?printf("w"):printf("-");
    (info.st_mode & S_IXUSR)?printf("x"):printf("-");
    (info.st_mode & S_IRGRP)?printf("r"):printf("-");
    (info.st_mode & S_IWGRP)?printf("w"):printf("-");
    (info.st_mode & S_IXGRP)?printf("x"):printf("-");
    (info.st_mode & S_IROTH)?printf("r"):printf("-");
    (info.st_mode & S_IWOTH)?printf("w"):printf("-");
    (info.st_mode & S_IXOTH)?printf("x"):printf("-");
    if(S_ISDIR(info.st_mode))
    {
        printf("\t%ld\t%s\t%s\t%ld\t\t%s\t"BLUE"%s\n"RESET,info.st_nlink,password->pw_name,gp->gr_name,info.st_size,time,filename);
    }
    else if((info.st_mode & S_IXUSR))
    {
        printf("\t%ld\t%s\t%s\t%ld\t\t%s\t"GREEN"%s\n"RESET,info.st_nlink,password->pw_name,gp->gr_name,info.st_size,time,filename);
    }
    else
    {
        printf("\t%ld\t%s\t%s\t%ld\t\t%s\t%s\n",info.st_nlink,password->pw_name,gp->gr_name,info.st_size,time,filename);
    }
}
void lsl(int dptr,int dne_ptr,int pdnd_ptr,int fptr,int a)
{
        if(fptr>0)
        {
            for(int i=0;i<fptr;i++)
            {
                print_lsl(".",files[i]);
            }
            printf("\n");
        }
        if(dptr>0)
        {
            for(int i=0;i<dptr;i++)
            {
                int ptr=0;
                DIR *dh1=opendir(dirs[i]);
                struct dirent *d1;
                char ** temp;
                int cnt=0;
                while((d1=readdir(dh1))!=NULL)
                cnt++;
                temp=(char **)malloc(cnt * sizeof(char*));
                dh1=opendir(dirs[i]);
                d1=readdir(dh1);
                printf("%s:\n",dirs[i]);
                long long total=0;
                char complete_filename[1000];
                while(d1!=NULL)
                {
                    if(d1->d_name[0]=='.' && a==0)
                    {
                        d1=readdir(dh1);
                        continue;
                    }
                    temp[ptr++]=(char*)malloc(sizeof(char)*strlen(d1->d_name)+1);
                    strcpy(temp[ptr-1],d1->d_name);
                    strcpy(complete_filename,dirs[i]);
                    strcat(complete_filename,"/");
                    strcat(complete_filename,d1->d_name);
                    struct stat info;
                    if(stat(complete_filename,&info)==0) total+=info.st_blocks;
                    d1=readdir(dh1);
                }
                qsort(temp,ptr,sizeof(char*),cmp);
                printf("total %lld\n",total/2);
                for(int j=0;j<ptr;j++)
                {
                    print_lsl(dirs[i],temp[j]);
                }
                printf("\n\n\n");
            }
        }
        if(dne_ptr>0)
        {
            for(int i=0;i<dne_ptr;i++)
            {
                printf("ls: cannot access '%s': No such file or directory\n\n",dne[i]);
            }
        }
}
void onlyls(char dir[],int a)
{
    printf("%s:\n",dir);
    int cnt=0,ptr=0;
    DIR *dh=opendir(dir);
    struct dirent *d=readdir(dh);
    while(d!=NULL)
    {
        cnt++;
        d=readdir(dh);
    }
    char **temp;
    temp=(char **)malloc(cnt * sizeof(char*));
    dh=opendir(dir);
    d=readdir(dh);
    while(d!=NULL)
    {
        if(d->d_name[0]=='.' && a==0)
        {
            d=readdir(dh);
            continue;
        }
        temp[ptr++]=(char*)malloc(sizeof(char)*strlen(d->d_name)+1);
        strcpy(temp[ptr-1],d->d_name);
        d=readdir(dh);
    }
    qsort(temp,ptr,sizeof(char *),cmp);
    for(int i=0;i<ptr;i++)
    {
        char complete_filename[1000];
        strcpy(complete_filename,dir);
        strcat(complete_filename,"/");
        strcat(complete_filename,temp[i]);
        struct stat info;
        stat(complete_filename,&info);
        if(S_ISDIR(info.st_mode))
        {
            printf(BLUE"%s\n"RESET,temp[i]);
        }
        else if((info.st_mode & S_IXUSR))
        {
            printf(GREEN"%s\n"RESET,temp[i]);
        }
        else
        {
            printf("%s\n",temp[i]);
        }
    }
}
void ls(char *parts[],char tilda[],int k)
{
    taamjaam();
    int a=0,l=0,dptr=0,dne_ptr=0,pdnd_ptr=0,fptr=0,tild=0;
    for(int i=0;i<k;i++)
    {
        if(strcmp(parts[i],"-a")==0)a=1;
        else if(strcmp(parts[i],"-l")==0)l=1;
        else if(strcmp(parts[i],"-la")==0||strcmp(parts[i],"-al")==0)a=1,l=1;
        else
        {
            DIR *dh=opendir(parts[i]);
            if(dh!=NULL) strcpy(dirs[dptr++],parts[i]);
            else
            {
                struct stat info1;
                char full_filename[2000];
                getcwd(full_filename,2000);
                strcat(full_filename,"/");
                strcat(full_filename,parts[i]);
                if(stat(full_filename,&info1)==0)
                {
                    strcpy(files[fptr++],parts[i]);
                    continue;
                }
                if(errno==ENOENT)
                {
                    if(strcmp("~",parts[i])==0)
                    {
                        strcpy(dirs[dptr++],tilda);
                        continue;
                    }
                    strcpy(dne[dne_ptr++],parts[i]);
                    continue;
                }
            }
        }
    }
    if(l==1)
    {
        if(dptr==0&&dne_ptr==0&&pdnd_ptr==0&&fptr==0)
        {
            strcpy(dirs[dptr++],".");
        }
        lsl(dptr,dne_ptr,pdnd_ptr,fptr,a);
    }
    else
    {
        if(dptr==0&&dne_ptr==0&&pdnd_ptr==0&&fptr==0)
        {
            onlyls(".",a);
        }
        for(int i=0;i<fptr;i++)
        {
            char complete_filename[1000];
            getcwd(complete_filename,1000);
            strcat(complete_filename,"/");
            strcat(complete_filename,files[i]);
            struct stat info;
            stat(complete_filename,&info);
            if((info.st_mode & S_IXUSR))
            {
                printf(GREEN"%s\n"RESET,files[i]);
            }
            else
            {
                printf("%s\n",files[i]);
            }
        }
        for(int i=0;i<dptr;i++)
        onlyls(dirs[i],a);
        for(int i=0;i<dne_ptr;i++)
        {
            printf("ls: cannot access '%s': No such file or directory\n\n",dne[i]);
        }
    }
}
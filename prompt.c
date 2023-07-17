#include "1.h"
#include "functions.h"

extern long time_took;

#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"

void prompt()
{
    char update[2000]="~";
    if(strcmp(curr,tilda)==0)
    {
        if(time_took!=-1)
        {
            printf(YELLOW"<%s@%s:"BLUE"%s took %lds>"RESET,user,hostname,update,time_took);
            time_took=-1;
        }
        else
        printf(YELLOW"<%s@%s:"BLUE"%s>"RESET,user,hostname,update);
        return;
    }
    if(strlen(curr)<strlen(tilda))
    {
        if(time_took!=-1)
        {
            printf(YELLOW"<%s@%s:"BLUE"%s took %lds>"RESET,user,hostname,curr,time_took);
            time_took=-1;
        }
        else
        printf(YELLOW"<%s@%s:"BLUE"%s>"RESET,user,hostname,curr);
        return;
    }
    int len=strlen(tilda),ok=1;
    for(int i=0;i<len;i++)
    {
        if(tilda[i]!=curr[i])
        {
            ok=0;
            break;
        }
    }
    if(ok==1)
    {
        int len2=strlen(curr),ptr=0;
        char temp[2000];
        for(int i=len;i<len2;i++)
        {
            temp[ptr++]=curr[i];
        }
        temp[ptr]='\0';
        strcat(update,temp);
        if(time_took!=-1)
        {
            printf(YELLOW"<%s@%s:"BLUE"%s took %lds>"RESET,user,hostname,update,time_took);
            time_took=-1;
        }
        else
        printf(YELLOW"<%s@%s:"BLUE"%s>"RESET,user,hostname,update);
    }
    else
    {
        if(time_took!=-1)
        {
            printf(YELLOW"<%s@%s:"BLUE"%s took %lds>"RESET,user,hostname,curr,time_took);
            time_took=-1;

        }
        printf(YELLOW"<%s@%s:"BLUE"%s>"RESET,user,hostname,curr);
    }
}
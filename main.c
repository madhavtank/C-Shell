#include "1.h"
#include "functions.h"

long time_took=-1;

#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define RESET "\x1b[0m"
char curr[2000],*user,hostname[2000],last_part_for_cd[2][2000],*parts[1000];
char *for_ls[2000];
int ok_last_cd=0;

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
void cd(char *dest)
{
    if(dest==0 || strcmp(dest,"~")==0)
    {
        chdir(tilda);
        ok_last_cd=1;
    }
    else if(strcmp(dest,"-")==0)
    {
        if(ok_last_cd==0)
        {
            printf("cd: OLDPWD not set\n");
            return;
        }
        printf("%s\n",last_part_for_cd[0]);
        chdir(last_part_for_cd[0]);
    }
    else if(chdir(dest)<0)
    {
        printf("cd: %s: No such file or directory\n",dest);
        return;
    }
    ok_last_cd=1;
    strcpy(last_part_for_cd[0],last_part_for_cd[1]);
    getcwd(last_part_for_cd[1],2000);
}

void bghandle()
{
    int status;
    int pid=waitpid(-1,&status,WNOHANG);
    if(pid>0)
    {
        int pos=0;
        while(pos<500 && bg_pid[pos]!=pid) pos++;
        if(pos>=500) return;
        if(WIFSTOPPED(status)) return;

        for(int i=0;i<bg_index;i++)
        {
            if(all_proc[i].pid==pid)
            {
                all_proc[i].run=0;
                break;
            }
        }

        fprintf(stderr,"\n%s with PID %d exited %s\n",bg_command[pos],bg_pid[pos],WIFEXITED(status) ? "normally" : "abnormally");
        bg_pid[pos]=0;
        free(bg_command[pos]);
    }
}

void control_c()
{
    pid_t pid=getpid();
    if(pid==current_id) return;
    kill(pid,SIGINT);
}

void control_z()
{

}

int main()
{
    bg_index=1;
    current_id=getpid();
    getcwd(tilda,2000);
    gethostname(hostname,2000);
    struct passwd *p;
    user=(char *)malloc(100);
    p=getpwuid(getuid());
    user=p->pw_name;
    char *command;
    size_t buff_size=1000;
    command=(char *)malloc(buff_size);
    strcpy(last_part_for_cd[1],tilda);
    while(1)
    {
        signal(SIGINT,control_c);
        signal(SIGTSTP,control_z);
        signal(SIGCHLD,bghandle);
        getcwd(curr,2000);
        prompt();
        // if(getline(&command,&buff_size,stdin)==-1)
        // {
        //     printf("\n");
        //     exit(0);
        // }
        command=get_input();
        printf("\n");
        if(strlen(command)>1)
        {
            // command[strlen(command)-1]='\0';
            FILE *file1,*file2;
            char full_z_address[2000];
            strcpy(full_z_address,tilda);
            strcat(full_z_address,"/z.txt");
            file1=fopen(full_z_address,"r");
            int i=0;
            char curr_his[25][1000];
            while(fscanf(file1,"%[^\n] ",curr_his[i])!=EOF) i++;
            if(i>=1 && strcmp(curr_his[i-1],command)!=0) {
            if(i==20)
            {
                for(int j=1;j<i;j++)
                {
                    strcpy(curr_his[j-1],curr_his[j]);
                }
                strcpy(curr_his[i-1],command);
            }
            else
            {
                strcpy(curr_his[i++],command);
            } }
            else if(i==0)
            {
                strcpy(curr_his[i++],command);
            }
            file2=fopen(full_z_address,"w");
            for(int j=0;j<i;j++)
            fprintf(file2,"%s\n",curr_his[j]);
            fclose(file1);
            fclose(file2);
        }
        // execute(command);
        // return 0;
        parts[0]=strtok(command,";");
        int i=0;
        while(parts[i]!=NULL)
        {
            i++;
            parts[i]=strtok(NULL,";");
        }
        for(int j=0;j<i;j++){

        char command_cpy[1000];
        strcpy(command_cpy,parts[j]);
        char *part1=strtok(parts[j]," ""\t");
        if(part1==NULL) continue;
        if(strcmp(part1,"\n")==0) continue;

        if(pipe_check(command_cpy))
        {
            handle_pipe(command_cpy);
            continue;
        }

        int redr=0;
        for(int r=0;r<strlen(command_cpy);r++)
        {
            if(command_cpy[r]=='<'||command_cpy[r]=='>')
            {
                redirection(command_cpy);
                redr=1;
                break;
            }
        }
        if(redr==1) continue;

        if(strcmp(part1,"cd")==0)
        {
            char *part2=strtok(NULL," ""\t");
            char *part3=strtok(NULL," ""\t");
            int k=0;
            while(part3!=0)
            {
                part3=strtok(NULL," ""\t");
                k++;
            }
            if(k>0)
            {
                printf("cd: too many arguments\n");
                continue;
            }
            cd(part2);
        }
        else if(strcmp(part1,"pwd")==0)
        {
            getcwd(curr,2000);
            printf("%s\n",curr);
        }
        else if(strcmp(part1,"echo")==0)
        {
            char *part2=strtok(NULL,"\n");
            if(part2==0)
            {
                printf("\n");
                continue;
            }
            char *part3=strtok(part2," ""\t");
            while(part3!=0)
            {
                printf("%s ",part3);
                part3=strtok(NULL," ""\t");
            }
            printf("\n");
        }
        else if(strcmp(part1,"ls")==0)
        {
            for_ls[0]=strtok(NULL," ""\t");
            int r=0;
            while(for_ls[r]!=NULL)
            {
                r++;
                for_ls[r]=strtok(NULL," ""\t");
            }
            ls(for_ls,tilda,r);
        }
        else if(strcmp(part1,"history")==0)
        {
            FILE *file1;
            char full_z_address[2000];
            strcpy(full_z_address,tilda);
            strcat(full_z_address,"/z.txt");
            file1=fopen(full_z_address,"r");
            int l=0;
            char curr_his[25][200];
            while(fscanf(file1,"%[^\n] ",curr_his[l])!=EOF) l++;
            for(int h=0;h<l;h++)
            printf("%s\n",curr_his[h]);
            fclose(file1);
        }
        else if(strcmp(part1,"pinfo")==0)
        {
            char *for_pinfo[10];
            for_pinfo[0]=strtok(NULL," ""\t");
            int r=0;
            while(for_pinfo[r]!=NULL)
            {
                r++;
                for_pinfo[r]=strtok(NULL," ""\t""\r");
            }
            pinfo(r,for_pinfo);
        }
        else if(strcmp(part1,"discover")==0)
        {
            char *for_disc[2000];
            for_disc[0]=strtok(NULL," ""\t");
            int r=0;
            while(for_disc[r]!=NULL)
            {
                r++;
                for_disc[r]=strtok(NULL," ""\t");
            }
            discover(r,for_disc);
        }
        else if(strcmp(part1,"exit")==0)
        exit(0);
        else if(strcmp(part1,"jobs")==0)
        {
            jobs(command_cpy);
        }
        else if(strcmp(part1,"sig")==0)
        {
            sig(command_cpy);
        }
        else if(strcmp(part1,"fg")==0)
        {
            fg(command_cpy);
        }
        else if(strcmp(part1,"bg")==0)
        {
            bg(command_cpy);
        }
        else
        {
            if(part1!=NULL)
            {
                char *for_fgbg[2000];
                for_fgbg[0]=part1;
                int r=1;
                for_fgbg[r]=strtok(NULL," ""\t");
                while(for_fgbg[r]!=NULL)
                {
                    r++;
                    for_fgbg[r]=strtok(NULL," ""\t");
                }
                fgbg(r,for_fgbg);
            }
        }
        }
    }
}
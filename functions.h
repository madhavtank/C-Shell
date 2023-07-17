#ifndef functions
#define functions

void ls(char *dir[],char tilda[],int k);
void fgbg(int k,char *parts[]);
void discover(int k,char *parts[]);
void pinfo(int k,char *parts[]);
void redirection(char command[]);
int pipe_check(char command[]);
void handle_pipe(char command[]);
void jobs();
void sig(char command[]);
void fg(char command[]);
char * ac(char *,char *,int *);
void bg(char command[]);
char* get_input();
void prompt();
void bgcall(int k,char *parts[]);
char toreturn[100];

#endif
#include <stdio.h>
#include <termios.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>


#include "1.h"
#include "functions.h"

void die(const char *s) {
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode() {
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you’re typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn’t been read.
 * The c_lflag field is for “local flags”
 */
void enableRawMode() {
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

/**
 * stdout and stdin are buffered we disable buffering on that
 * After entering in raw mode we read characters one by one
 * Up arrow keys and down arrow keys are represented by 3 byte escape codes
 * starting with ascii number 27 i.e. ESC key
 * This way we interpret arrow keys
 * Tabs are usually handled by the term, but here we are simulating tabs for the sake of simplicity
 * Backspace move the cursor one control character to the left
 * @return
 */
char *get_input(){
    char *inp = malloc(sizeof(char) * 100);
    char *after_space=malloc(sizeof(char)*100);
    memset(after_space,'\0',100);
    char c;
    // while (1) {
        setbuf(stdout, NULL);
        enableRawMode();
        memset(inp, '\0', 100);
        int pt = 0,pt2=0;
        while (read(STDIN_FILENO, &c, 1) == 1) {
            if(c==32)
            {
                memset(after_space,'\0',100);
                pt2=0;
            }
            else if(c!=9 && c!=127)
            after_space[pt2++]=c;
            if (iscntrl(c)) {
                if (c == 10) {break;}
                else if (c == 27) {
                    char buf[3];
                    buf[2] = 0;
                    if (read(STDIN_FILENO, buf, 2) == 2) { // length of escape code
                        printf("\rarrow key: %s", buf);
                    }
                } else if (c == 127) { // backspace
                    if (pt > 0) {
                        if (inp[pt-1] == 9) {
                            for (int i = 0; i < 7; i++) {
                                printf("\b");
                            }
                        }
                        inp[--pt] = '\0';
                        if(pt2);
                        after_space[--pt2]='\0';
                        printf("\b \b");
                    }
                } else if (c == 9) { // TAB character
                    // inp[pt++] = c;
                    // for (int i = 0; i < 8; i++) { // TABS should be 8 spaces
                    //     printf(" ");
                    
                    int no_match=0;

                    if(ac(inp,after_space,&no_match)!=NULL)
                    {
                        if(singal_match!=1)
                        {
                            printf("\n");
                            prompt();
                            strcat(inp,toreturn);
                            strcat(after_space,toreturn);
                            pt+=strlen(toreturn);
                            pt2+=strlen(toreturn);
                            printf("%s",inp);
                        }
                        else
                        {
                            strcat(inp,toreturn);
                            strcat(after_space,toreturn);
                            printf("%s",toreturn);
                            pt+=strlen(toreturn);
                            pt2+=strlen(toreturn);
                        }
                    }
                    else
                    {
                        if(no_match==1)
                        {
                            printf("\n");
                            prompt();
                            if(strcmp(inp,"")==0)
                            printf(" ");
                            printf("%s",inp);
                        }
                    }
                    singal_match=0;
                }
                // else if(c==32)
                // {
                //     pt2=0;
                //     memset(after_space,'\0',100);
                // }
                 else if (c == 4) {
                    exit(0);
                } else {
                    printf("%d\n", c);
                }
            } else {
                inp[pt++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();

    // }
    return inp;
}
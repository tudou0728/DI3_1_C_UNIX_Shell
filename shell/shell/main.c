#include <pwd.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include "shell_fct.h"
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/history.h>
/**
*attention:chaque fois , appuyer "Barre d'espace"  avant appuyer "Enter" pour executer,
*attention:chaque fois , appuyer "Barre d'espace"  avant appuyer "Enter" pour executer,
*attention:chaque fois , appuyer "Barre d'espace"  avant appuyer "Enter" pour executer,
*ex1:commande:ls(espace)-l(espace)(Enter)
*ex2:commande:echo(espace)absdef(espace)(Enter)
*/

//To complete
int status;
pid_t pi;
char* readlineptr;
void alarmHandler_pere(int sigNum);
int main(int argc, char** argv)
{
	//..........
	int ret = MYSHELL_CMD_OK;
	//char* readlineptr;
	struct passwd* infos;
	char str[1024];
	char hostname[256];
	char workingdirectory[256];
	//..........
    while((ret != MYSHELL_FCT_EXIT))
    {
        //Get your session info
        infos=getpwuid(getuid());
		gethostname(hostname, 256);
		getcwd(workingdirectory, 256);
        cmd*c;
        //Print it to the console
		sprintf(str, "\n{myshell}%s@%s:%s$ ", infos->pw_name, hostname, workingdirectory);
		readlineptr = readline(str);
        //Your code goes here.......
        add_history(readlineptr);
        //rl_bind_key('\t', rl_complete(readlineptr,rl_completion_matches));
        //rl_attempted_completion_function = completion;
        rl_bind_key('\t', rl_complete);
        //Parse the comand
        c=(cmd*)malloc(sizeof(cmd));
        parse_members(readlineptr,c);
        print_members(c);
        printf("\n");
        parse_members_args(c);
        print_members_args(c);
        printf("\n");

        unsigned  int memberscount=c->nb_cmd_members;
        c->redirection=(char***)malloc(sizeof(char**) * memberscount);
        unsigned int a=0;
        c->redirection_type=(int**)malloc(sizeof(int*) * memberscount);
        int k=0;
        for(k=0;k<memberscount;k++)
        { c->redirection_type[k]=(int*)malloc(sizeof(int));}
        for(a=0;a<memberscount;a++)
        {
           parse_redirection(a,c);
        }
        int b=0;
        for(b=0;b<memberscount;b++)
        {
           print_redirection(c,b);
           printf("\n");
        }
        //Execute the comand
        pi=fork();
        if(pi == 0)
        {
		  exec_command(c);
		  free_members(c);
          free_members_args(c);
          free_redirection(c);
          free(c);
          free(readlineptr);
        }
        else
        {
        signal(SIGALRM, alarmHandler_pere);
        alarm(5);
        wait(&status);
        if(WIFEXITED(status))
        { printf("Child process successfully completed\n");}
        else
        { printf("Child process unsuccessfully completed\n");}
        }
    }
	//..........
	return 0;
}

void alarmHandler_pere(int sigNum)
{
	printf("Alarm handled: killing child process\n");
	printf("\n");
	kill(pi, SIGKILL);
}


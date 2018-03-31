#include "shell_fct.h"
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>


void execute_a_process(int**tube,pid_t*pid,unsigned int i,cmd*cc)
{
    pipe(tube[i]);
    pid[i]=fork();
    if(pid[i] == 0)
    {
      close(tube[i-1][1]);
      dup2(tube[i-1][0], 0);
      close(tube[i-1][0]);

      close(tube[i][0]);
      dup2(tube[i][1], 1);
      close(tube[i][1]);

      if(execvp(cc->cmd_members_args[i][0],cc->cmd_members_args[i]) == -1)
      {
        perror("execvp");
        exit(errno);
      }
    }
    else
    {
       close(tube[i-1][0]);
       close(tube[i-1][1]);
    }
}


int exec_command(cmd* my_cmd)
{
    unsigned int memberscout=my_cmd->nb_cmd_members;
    int length=strlen(my_cmd->init_cmd);
    char buffer[50];
    //申请管道
    int **tube;
    tube=(int**)malloc(sizeof(int*) * memberscout);
    int a=0;
    for(a=0;a<memberscout;a++)
    { tube[a]=(int*)malloc(sizeof(int) *2);}
    //申请进程
    pid_t *pid;
    pid=(pid_t*)malloc(sizeof(pid_t) * memberscout);

    pipe(tube[0]);
    pid[0]=fork();
    int m=0;
    if(pid[m] == 0) //第一个进程
    {
       //sleep(10);
       // signal(SIGALRM,alarmHandler_file);
        if((my_cmd->redirection[m][0] == NULL))
        {
           close(tube[m][0]);
           dup2(tube[m][1], 1);
           close(tube[m][1]);
           //sleep(10);
           if(execvp(my_cmd->cmd_members_args[m][0],my_cmd->cmd_members_args[m]) == -1)
           {
             perror("execvp");
             exit(errno);
           }
           //sleep(10);
        }
        else
        {
           int fd=open(my_cmd->redirection[m][0], O_RDWR|O_CREAT, S_IRWXU);
           dup2(fd, 0);
           close(tube[m][0]);
           dup2(tube[m][1], 1);
           close(tube[m][1]);
           if(execvp(my_cmd->cmd_members_args[m][0],my_cmd->cmd_members_args[m]) == -1)
           {
             perror("execvp");
             exit(errno);
           }
        }
    }
    else
    {
      //执行多个进程
      while(m < memberscout-1)
      {
        // signal(SIGALRM,alarmHandler_file);
         m++;
         execute_a_process(tube,pid,m,my_cmd);
      }
      //最后的输出
      close(tube[m][1]);
      dup2(tube[m][0],0);
      close(tube[m][0]);
      if((my_cmd->redirection[m][1] != NULL) && (my_cmd->redirection_type[m][0]==1))
      {
         int fd=open(my_cmd->redirection[m][1], O_RDWR|O_CREAT, S_IRWXU);
         lseek(fd,0,SEEK_END);
         dup2(fd, 1);
      }
      else if((my_cmd->redirection[m][1] != NULL) & (my_cmd->redirection_type[m][0]==2))
      {
         int fd=open(my_cmd->redirection[m][1], O_RDWR|O_CREAT, S_IRWXU);
         dup2(fd, 1);
      }
      //标准错误有点儿问题
      else if((my_cmd->redirection[m][2] != NULL) && (my_cmd->redirection_type[m][0]==1))
      {
         int fd=open(my_cmd->redirection[m][1], O_RDWR|O_CREAT, S_IRWXU);
         lseek(fd,0,SEEK_END);
         dup2(fd, 2);
      }
      else if((my_cmd->redirection[m][2] != NULL) && (my_cmd->redirection_type[m][0]==2))
      {
         int fd=open(my_cmd->redirection[m][1], O_RDWR|O_CREAT, S_IRWXU);
         dup2(fd, 2);
      }

      int * status;
      status=(int*)malloc(sizeof(int) * memberscout);
      int d=0;
      while(d<memberscout)
      {
        waitpid(pid[d],&(status[d]),0);
        d++;
      }

      while(fgets(buffer, length, stdin) != NULL)
      { printf("%s", buffer); }
    }
    int n=0;
    for(n=0;n<memberscout;n++)
    {
       free(tube[n]);
    }
    free(pid);
    free(tube);
    exit(EXIT_SUCCESS);
}



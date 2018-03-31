#include "cmd.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//settings:-lreadline -lpthread
//Your includes come here
/**redirection_type:
*0:no out-redirection
*1:append
*2:override
*/

//Remplit les champs initial_cmd, membres_cmd et nb_membres
void parse_members(char *s,cmd *c)
{
   //init_cmd;
    c->init_cmd=(char*)malloc(sizeof(char) * 50);
    c->init_cmd=strcpy(c->init_cmd,s);
    //nb_cmd_members
    unsigned int memberscount=0;
    int i=0;
    while(s[i] != '\0')
    {
       if(s[i] != '|')
       { i++; }
       else
       {
          i++;
          memberscount++;
       }
    }
    memberscount++;
    c->nb_cmd_members=memberscount;

    //members_cmd
    c->cmd_members=(char**)malloc(sizeof(char*)*memberscount);
    unsigned int m=0;
    unsigned int n=0;
    while(m < (memberscount))
    {
        int p=0;
        c->cmd_members[m]=(char*)malloc(sizeof(char)*40);
        while((s[n] != '|') && (s[n] != '\0'))
        {
            if((s[n] == 'char(32)') && (s[n+1] == '|'))
            {n++;}
            else
            {
               c->cmd_members[m][p]=s[n];
               n++;
               p++;
            }
        }
        c->cmd_members[m][p]='\0';
        m++;
        p=0;
        n=n+2;
    }
    //your implementation comes here
}

//Prints the contents of members to the console
void print_members(cmd *c)
{
    printf("nb_cmd_members=%d\n",(c->nb_cmd_members));
    unsigned int i=0;
    while(i < (c->nb_cmd_members))
    {
       printf("cmd_members[%d]=%s\n",i,(c->cmd_members[i]));
       i++;
    }
    //your implementation comes here
}

//Frees the memory allocated to store member information
void free_members(cmd *c)
{
    unsigned int i=0;
    while(i<(c->nb_cmd_members))
    {
       free(c->cmd_members[i]);
       i++;
    }
    free(c->cmd_members);
    free(c->init_cmd);
    //your implementation comes here
}

int countargs(unsigned int i,cmd*c)
{
    int k=0;
    int count=1;
    while((c->cmd_members[i][k] != '\0') && (c->cmd_members[i][k] != '<') && (c->cmd_members[i][k] != '>') && (c->cmd_members[i][k] != '2'))
    {
        if(c->cmd_members[i][k] == ' ')
        {
           count++;
           k++;
        }
        else
        { k++; }
    }
    return count;
}
//Remplit les champs cmd_args et nb_args_membres

void parse_members_args(cmd *c)
{
    unsigned int memberscount=c->nb_cmd_members;
    int length=strlen(c->init_cmd);

    c->cmd_members_args=(char***)malloc(sizeof(char**) * (memberscount));
    c->nb_members_args=(int*)malloc(sizeof(int) * memberscount);

    unsigned int i=0;
    unsigned int j=0;
    while(i < (memberscount))
    {
      c->cmd_members_args[i]=(char**)malloc(sizeof(char*) * countargs(i,c));
      int a=0;
      int b=0;
      c->cmd_members_args[i][j]=(char*)malloc(sizeof(char) * length);
      while((c->cmd_members[i][a] != '\0') && (c->cmd_members[i][a] != '<') && (c->cmd_members[i][a] != '>') && (c->cmd_members[i][a] != '2'))
      {
         //c->cmd_members_args[i][j]=(char*)malloc(sizeof(char) * 30);
         if(c->cmd_members[i][a] != ' ')
         {
             c->cmd_members_args[i][j][b]=c->cmd_members[i][a];
             b++;
             a++;
         }
         else
         {
           c->cmd_members_args[i][j][b]='\0';
           a++;
           j++;
           c->cmd_members_args[i][j]=(char*)malloc(sizeof(char) * length);
           b=0;
         }
       }
      c->nb_members_args[i]=j;
      c->cmd_members_args[i][j]=NULL;
      j=0;
      i++;
    }
    //your implementation comes here
}

//Prints the contents of members_args to the console
void print_members_args(cmd *c)
{
   unsigned int i=0;
   unsigned int j=0;
   while(i<(c->nb_cmd_members))
   {
      printf("nb_members_args[%d]=%d\n",i,(c->nb_members_args[i]));
      while(j<=(c->nb_members_args[i]))
      {
        printf("cmd_members_args[%d][%d]=%s\n",i,j,(c->cmd_members_args[i][j]));
        j++;
      }
     // printf("cmd_members_args[%d][%d]=%s\n",i,j,"null");
      i++;
      j=0;
   }
    //your implementation comes here
}

//Frees the memory allocated to store member arguments
void free_members_args(cmd *c)
{
   unsigned int i=0;
   unsigned int j=0;
   while(i < (c->nb_cmd_members))
   {
      while(j <= (c->nb_members_args[i]))
      {
        free(c->cmd_members_args[i][j]);
        c->cmd_members_args[i][j]=NULL;
        j++;
      }
      free(c->cmd_members_args[i]);
      c->cmd_members_args[i]=NULL;
      free(c->nb_members_args[i]);
      c->nb_members_args[i]=NULL;
      i++;
   }
   free(c->cmd_members_args);
   c->cmd_members_args=NULL;
   free(c->init_cmd);
   c->init_cmd=NULL;
    //your implementation comes here
}

//Remplit les champs redir et type_redir
void parse_redirection(unsigned int i, cmd *c)
{
    c->redirection[i]=(char**)malloc(sizeof(char*) * 3);
    unsigned int m=0;
    unsigned int n=0;
    int length=strlen(c->init_cmd);
    int k=0;
    for(k=0;k<3;k++)
    {c->redirection[i][k]=(char*)malloc(sizeof(char) * length);}
    //c->redirection[i][0]=(char*)malloc(sizeof(char) * 30);
    //c->redirection[i][1]=(char*)malloc(sizeof(char) * 30);
    //c->redirection[i][2]=(char*)malloc(sizeof(char) * 30);

    while(c->cmd_members[i][m] != '\0')
    {
       if(c->cmd_members[i][m] == '<')
       {
           m=m+2;
           while(c->cmd_members[i][m] != '\0')
           {
             c->redirection[i][0][n]=c->cmd_members[i][m];
             n++;
             m++;
           }
          c->redirection[i][0][n]='\0';
          c->redirection[i][1]=NULL;
          c->redirection[i][2]=NULL;
          break;
       }

       else if((c->cmd_members[i][m] == '>') && (c->cmd_members[i][m+1] == '>'))
       {
           m=m+3;
           while(c->cmd_members[i][m] != '\0')
           {
             c->redirection[i][1][n]=c->cmd_members[i][m];
             n++;
             m++;
           }
          c->redirection[i][1][n]='\0';
          c->redirection_type[i][0]=1;
          c->redirection[i][0]=NULL;
          c->redirection[i][2]=NULL;
          break;
       }

       else if((c->cmd_members[i][m] == '>') && (c->cmd_members[i][m+1] != '>'))
       {
           m=m+2;
           while(c->cmd_members[i][m] != '\0')
           {
             c->redirection[i][1][n]=c->cmd_members[i][m];
             n++;
             m++;
           }
          c->redirection[i][1][n]='\0';
          c->redirection_type[i][0]=2;
          c->redirection[i][0]=NULL;
          c->redirection[i][2]=NULL;
          break;
       }

       else if((c->cmd_members[i][m] == '2') && (c->cmd_members[i][m+1] == '>') && (c->cmd_members[i][m+2] != '>'))
       {
           m=m+3;
           while(c->cmd_members[i][m] != '\0')
           {
             c->redirection[i][2][n]=c->cmd_members[i][m];
             n++;
             m++;
           }
          c->redirection[i][2][n]='\0';
          c->redirection_type[i][0]=2;
          c->redirection[i][0]=NULL;
          c->redirection[i][1]=NULL;
          break;
       }

       else if((c->cmd_members[i][m] == '2') && (c->cmd_members[i][m+1] == '>') && (c->cmd_members[i][m+2] == '>'))
       {
           m=m+4;
           while(c->cmd_members[i][m] != '\0')
           {
             c->redirection[i][2][n]=c->cmd_members[i][m];
             n++;
             m++;
           }
          c->redirection[i][2][n]='\0';
          c->redirection_type[i][0]=1;
          c->redirection[i][0]=NULL;
          c->redirection[i][1]=NULL;
          break;
       }
       else
       { m++;
       }
    }
    char *temp2;
    temp2=index(c->cmd_members[i],'>');
    char *temp1;
    temp1=strchr(c->cmd_members[i],'<');
    if((temp1 == NULL) && (temp2 == NULL))
    {
       c->redirection[i][0]=NULL;
       c->redirection[i][1]=NULL;
       c->redirection[i][2]=NULL;
    }
    if(temp2 == NULL)
    { c->redirection_type[i][0]=0;}
}

//Prints the redirection information for member i
void print_redirection(cmd*c, int i)
{
    int j=0;
    for(j=0;j<3;j++)
    { printf("redirection[%d][%d]=%s\n",i,j,c->redirection[i][j]);}
    if(c->redirection_type[i][0] == 0)
    { printf("redirection_type[%d][0]=NULL\n",i,c->redirection_type[i][0]);}
    else if(c->redirection_type[i][0] == 1)
    { printf("redirection_type[%d][0]=Append\n",i,c->redirection_type[i][0]);}
    else if(c->redirection_type[i][0] == 2)
    { printf("redirection_type[%d][0]=Override\n",i,c->redirection_type[i][0]);}
    //printf("\n");
    //your implementation comes here
}

//Frees the memory allocated to store redirection info
void free_redirection(cmd *c)
{
    unsigned int i=0;
    while(i < (c->nb_cmd_members))
    {
       int j=0;
       while(j<3)
       {
          free(c->redirection[i][j]);
          c->redirection[i][j]=NULL;
          j++;
       }
       free(c->redirection[i]);
       c->redirection[i]=NULL;
       free(c->redirection_type[i]);
       c->redirection_type[i]=NULL;
       i++;
    }
    free(c->redirection);
    c->redirection=NULL;
    free(c->redirection_type);
    c->redirection_type=NULL;
    //your implementation comes here
}

//char** changform(cmd *c)

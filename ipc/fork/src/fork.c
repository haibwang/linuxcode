#include<unistd.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include"colors.h"

void usages(char **argv)
{
	printf(YELLOW"USAGES:%s \"chils_process_path\"\n"NONE,argv[0]);
}


void get_basename(char *pathname,char *basename)
{
	char i = 0;
	char j = 0;

	while(pathname[i])
	{
	    if((pathname[i]=='\\') || (pathname[i]=='/'))
		{
			j = i + 1;
		}

	    i++;
	}

	memcpy(basename, &pathname[j], i - j + 1);
}

int main(int argc, char **argv)
{
	pid_t pid;
    char basename[30];
    char *arg_vec[2];
	
	if(argc < 2)
	{
		usages(argv);
		exit(1);
	}


	get_basename(argv[1], basename);
	printf("basenmae is : %s\n", basename);
    
	arg_vec[0] = malloc(sizeof(char)*strlen(basename));
	memcpy(arg_vec[0], basename, strlen(basename));
    arg_vec[1]=NULL;


	//this is to fork a process
	pid = fork();

	switch(pid)
	{
		case -1:
			{
				printf("fork failed\n");
			
				break;
			}
		case 0:
			{
				//child process

				execve(argv[1], arg_vec, NULL);

				break;
			}
		default:
			{
				wait(NULL);
				printf(BLUE"child process run over\n"NONE); 
				break;
			}
	}
	return 0;
}




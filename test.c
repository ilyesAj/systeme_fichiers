#include <stdio.h>
#include<unistd.h>
#include<sys/signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include<stdlib.h>
#include <time.h>




int main(int argc, char *argv[], char *envp[])
{
	int pid;
	char *newargv[] = { NULL };
	char *newenviron[] = { NULL };
	pid = fork();
	if (pid == 0 )
	{
		execve ("term",newargv,newenviron);
	}
	else
	{
		wait(NULL);
		printf("fin fils\n");
		exit(0);
	}
	return 0;
}

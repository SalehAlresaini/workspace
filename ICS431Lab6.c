
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#define SHMSIZE 30

void noHandler(){

}

int main()
{
	int shmid;
	int pid0 =getpid(), pid1, pid2;
	signal(SIGUSR1, noHandler);
	if(pid1=fork() == 0)
	{	
		char *s, *shm;
		shmid = shmget(0x5678, SHMSIZE, 0666 | IPC_CREAT);
		shm = shmat(shmid, NULL, 0);
		printf("Enter a sentence: >");
		fgets(s, SHMSIZE, stdin);
		strcpy(shm,s);
		kill(pid0, SIGUSR1);
		pause();
		printf("Message '%s' sent from p1, Message '%s' receved from p2 \n",s,shm );
		shmdt (shm);
		shmctl(shmid, IPC_RMID, NULL);
	}
	else{
	pause();
		if(pid2=fork() == 0)
		{	
			char *s, *shm;
			shmid = shmget(0x5678, SHMSIZE, 0666);
			shm = shmat(shmid, NULL, 0);
			s =shm;
			while(*s != '\0'){
				if(*s >= 'a' && *s <= 'z')
					*s -=32;
				*s++;
			}
		}
		else
		{
			wait(NULL);
			kill(pid1, SIGUSR1);
			wait(NULL);
		}
	}
	return 0;
}

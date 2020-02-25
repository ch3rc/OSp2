//=======================================================================
//Date:		February 15,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		handleChild.c
//=======================================================================

#include "oss.h"

//=========================Primality prototype===========================

void primeNum(int);

//========================Shared Memory prototypes=======================
struct clock *getClock(key_t key, size_t size, int *shmid);
struct message *returnMessage(key_t key, size_t size, int *shmid);
int *childResults(key_t key, size_t size, int *shmid);
void quitTime(int, siginfo_t *, void *);
void cleanMem(void *);

//========================Shared Memory Pointers=========================

key_t key = CLOCK;
key_t key2 = MESSAGE;
key_t key3 = ARRAY;

size_t clockSize = sizeof(struct clock *);
size_t msgSize = sizeof(struct message *);


int clockId = 0;
int msgId = 0;
int arrId = 0;


struct clock *clockPtr = NULL;
struct message *msgPtr = NULL;
int *arrPtr = NULL;
//========================================================================

int startSec;
int startNano;
int numChildren;
struct sigaction quit;

int main(int argc, char *argv[])
{
	quit.sa_sigaction = quitTime;
	quit.sa_flags = 0;
	if(sigaction(SIGTERM, &quit, NULL) == -1)
		perror("handleChild: ERROR: SIGQUIT");
		
	printf("getting shared memory\n\n");

	clockPtr = getClock(key, clockSize, &clockId);
	msgPtr = returnMessage(key2, msgSize, &msgId);
	arrPtr = childResults(key3, numChildren * sizeof(int), &arrId);		

	printf("sec = %d nano = %d args = %d, arg0 = %s, arg1 = %s\n\n", clockPtr->sec, clockPtr->nano, argc, argv[0], argv[1]);

	msgPtr->id = atoi(argv[1]);
	msgPtr->num = atoi(argv[1]);

	int num = atoi(argv[0]);

	startNano = clockPtr->nano;
	
	//sleep(1);		
	
	primeNum(num);

	printf("detaching shared memory in handleChild\n\n");
	
	if(shmdt((void *)clockPtr) == -1)
		perror("handleChild: ERROR: shmdt (clockPtr)");

	if(shmdt((void *)msgPtr) == -1)
		perror("handleChild: ERROR: shmdt (msgPtr)");

	if(shmdt((void *)arrPtr) == -1)
		perror("handleChild: ERROR: shmdt (arrPtr)");

	printf("handleChild process complete\n\n");	
	raise(SIGCHLD);
	exit(2);
	
}

//========================Shared Memory Functions=================================

struct clock *getClock(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, 0666);
	if(*shmid < 0)
	{
		perror("handleChild: ERROR: shmget (getClock function)");
		exit(1);
	}

	struct clock *temp = (struct clock *)shmat(*shmid, NULL, 0);
	if(temp == (void *)-1)
	{
		perror("handleChild: ERROR: shmat (getClock function)");
		exit(1);
	}

	return temp;
}

struct message *returnMessage(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, 0666);
	if(*shmid < 0)
	{
		perror("handleChild: ERROR: shmget (returnMessage function)");
		exit(1);
	}

	struct message *temp = (struct message *)shmat(*shmid, NULL, 0);
	
	if(temp == (void *)-1)
	{
		perror("handleChild: ERROR: shmat (returnMessage function)");
		exit(1);
	}

	return temp;

}

int *childResults(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, 0666);
	if(*shmid < 0)
	{
		perror("handleChild: ERROR: shmget (childResults)");
		exit(1);
	}

	int *temp = (int *)shmat(*shmid, NULL, 0);
		
	if(temp == (void *)-1)
	{
		perror("handleChild: ERROR: shmat (childResults)");
		exit(1);
	}
		
	return temp;
}

void cleanMem(void *ptr)
{
	shmdt((void *) ptr);
}

//==========================Signal Function=============================================
void quitTime(int signo, siginfo_t *info, void *ptr)
{
	char msg[] = "ENTERED into handleChild quit\n\n";
	int msgLen = sizeof(msg);
	write(STDERR_FILENO, msg, msgLen);

	shmdt((void *)clockPtr);
	shmdt((void *)msgPtr);
	shmdt((void *)arrPtr);

	exit(0);
}

//===========================Prime Number Function=======================================
void primeNum(int number)
{
	
	int i;
	int flag = 0;
	for(i = 2; i <= number/2; i++)
	{
		if(number % i == 0)
		{
			flag = 1;
			break;
		}
	}

	if((clockPtr->nano - startNano) > 1000000)
	{
		arrPtr[msgPtr->id - 1] =  -1;
	}
	
	else if(flag == 0)
	{
		arrPtr[msgPtr->id - 1] =  number;
	}
	else if(flag == 1)
	{
		arrPtr[msgPtr->id - 1] =  -number;
	}
}

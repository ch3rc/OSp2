//=======================================================================
//Date:		February 15,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		handleChild.c
//=======================================================================

#include "oss.h"

struct clock *getClock(key_t key, size_t size, int *shmid);

void main(int argc, char *argv[])
{
 	key_t key = TESTKEY;
	size_t clockSize = sizeof(struct clock *);
	int clockId;
	struct clock *clockPtr = NULL;

	printf("getting shared memory\n\n");

	clockPtr = getClock(key, sizeof(struct clock *), &clockId);
	
	/*clockId = shmget(key, sizeof(struct clock *), 0666);
	if(clockId < 0)
	{
		perror("shmget client");
		exit(1);
	}

	clockPtr = (struct clock *)shmat(clockId, NULL, 0);
	if(clockPtr == (void *)-1)
	{
		perror("shmat client");
		exit(1);
	}*/

	clockPtr->status = RUN;

	sleep(1);

	printf("sec = %d nano = %d args = %d, arg0 = %s, arg1 = %s\n", clockPtr->sec, clockPtr->nano, argc, argv[0], argv[1]);

	printf("detaching shared memory and exiting handleChild.c\n\n");
	
	clockPtr->status = FIN;	

	if(shmdt((void *)clockPtr) == -1)
		perror("handleChild shmdt");

	printf("handleChild process complete\n");	

	exit(0);
	
}

struct clock *getClock(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, 0666);
	if(*shmid < 0)
	{
		perror("shmget");
		exit(1);
	}

	struct clock *temp = (struct clock *)shmat(*shmid, NULL, 0);
	if(temp == (void *)-1)
	{
		perror("shmat");
		exit(1);
	}

	return temp;
}

//==============================================================================
//Date:		February 14,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		oss.c
//==============================================================================

#include "oss.h"

#define PERM (IPC_CREAT | 0666)

int numChildren = 4;
int allowedChildren = 2;
int number;
int incrementBy;
char *filename;

int h_help;
int n_maxChild;
int s_allowedChildren;
int b_startNum;
int i_increment;
int o_outputFile;

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

pid_t childPid = 0;
int exitPid = 0;

int status;
int *pids;
int pidSize = 0;
int i = 0;
int j = 1;
int dead = 0;
int primes = 0;
int nonPrimes = 0;
int launched = 0;
int *nums;
struct sigaction act, catch;

//============================================================================================

struct clock *clockMem(key_t key, size_t size, int *shmid);
struct message *message(key_t key, size_t size, int *shmid);
int *childResults(key_t key, size_t size, int *shmid);
void cleanUp(void *, int);
void timesUp(int, siginfo_t *, void *);
void userEnd(int, siginfo_t *, void *);
static int setAlarm();

//============================================================================================

int main(int argc, char *argv[])
{	
	int prime[numChildren];
	int nonPrime[numChildren];

	initOpt();

	setOpt(argc, argv);

	if(h_help)
	{
		help();
	}

	//signal handler for two second alarm
	act.sa_sigaction = timesUp;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGALRM, &act, NULL);
	
	//signal handler for ctrl+c
	catch.sa_sigaction = userEnd;
	sigemptyset(&catch.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &catch, NULL);
	
	//memory allocations for clock, return message, and result array
	clockPtr = clockMem(key, clockSize, &clockId);
	msgPtr = message(key2, msgSize, &msgId);
	arrPtr = childResults(key3, numChildren * sizeof(int), &arrId);

	//init result array spots to zero
	for(i = 0; i < numChildren; i++)
	{
		arrPtr[i] = 0;
	}
	
	//create array to restrict max amount of pids allowed
	for(i = 0; i < allowedChildren; i++)
	{
		pidSize++;
		pids = realloc(pids, pidSize);
		pids[i] = 0;
	}


	setAlarm();
	
	FILE *fp = fopen(filename, "a");
	if(fp == NULL)
	{
		perror("file");
		exit(1);
	}
	
	while(1)
	{	
		//start clock	
		clockPtr->nano += 1;
		if(clockPtr->nano == 100000000)
		{
			clockPtr->sec += 1;
			clockPtr->nano = 0;
		}		
		
		//launch max allowed children in system at one time
		if(launched < allowedChildren && dead != numChildren)
		{
			childPid = fork();
			if(childPid < 0)
			{
				perror("fork");
				exit(1);
			}

			if(childPid == 0)
			{
				char str[25];
				char id[25];
				snprintf(str, sizeof(str), "%d", number);
				snprintf(id, sizeof(id), "%d", j);
				execlp("./handleChild", str, id, NULL);
			}
			//add children to pid array and store time of launch
			launched++;
			fprintf(fp,"child %d launched at %d nano %d sec\n\n", j, clockPtr->nano, clockPtr->sec);
			pids[launched - 1] = childPid;
			j++;
			number += incrementBy;
			
		}
			//see if child has exited and mark time if it did
		for(i = 0; i < pidSize; i++)
		{	
			if((exitPid = waitpid(pids[i], &status, 0)) > 0)
			{	
				if(WIFEXITED(status))
				{
					if(WIFEXITED(status))
					{
					fprintf(fp,"child %d finished at %d nano %d sec with result %d\n\n", msgPtr->id, clockPtr->nano, clockPtr->sec, arrPtr[msgPtr->id - 1]);
						
					//store primes/nonprimes in arrays to display at end of file
					if(arrPtr[msgPtr->id - 1] > 0)
					{
						prime[primes] = arrPtr[msgPtr->id - 1];
						primes++;
					}
					else if(arrPtr[msgPtr->id - 1] < 0 && arrPtr[msgPtr->id -1] != 1)
					{
						nonPrime[nonPrimes] = arrPtr[msgPtr->id - 1];
						nonPrimes++;
					}
					dead++;
					pids[i] = 0;
					launched--;
					}
				}
	
			}
		
		}
		
		if(dead == numChildren)
		{
			break;
		}

		
				
	}
	//write primes to file	
	for(i = 0; i < primes; i++)
	{
		fprintf(fp,"primes %d\n", prime[i]);
	}
	//write non-primes to file
	for(i = 0; i < nonPrimes; i++)
	{
		fprintf(fp,"nonPrimes %d\n", nonPrime[i]);
	}

	cleanUp(clockPtr, clockId);
	cleanUp(msgPtr, msgId);
	cleanUp(arrPtr, arrId);
	fclose(fp);
	
	//print();
	
	printf("process finished and written to %s\n", filename);
	
	return 0;
}

//=========================================Getopt==================================================

void initOpt()
{
	h_help = 0;
	n_maxChild = 0;
	s_allowedChildren = 0;
	b_startNum = 0;
	i_increment = 0;
	o_outputFile = 0;
}


void help()
{
	printf("--------Help----------\n");
	printf("\n\n");
	printf("-n: Indicate max total of child processes OSS will ever create\n");
	printf("-s: Indicate number of children allowed to exist in the system at the same time\n");
	printf("-b: Start of the sequence of numbers to be tested for primality\n");
	printf("-i: Increment between numbers that we test\n");
	printf("-o: Create an output file to show results\n");
}


void setOpt(int argc, char *argv[])
{
	int c;

	while((c = getopt(argc, argv, "hn:s:b:i:o:")) != -1)
	{
		switch(c)
		{
			case 'h':
				h_help = 1;
				break;

			case 'n':
				n_maxChild = 1;
				numChildren = atoi(optarg);
				if(numChildren > 20)
				{
					numChildren = 20;
				}
				break;
			
			case 's':
				s_allowedChildren = 1;
				allowedChildren = atoi(optarg);
				break;

			case 'b':
				b_startNum = 1;
				number = atoi(optarg);
				break;

			case 'i':
				i_increment = 1;
				incrementBy = atoi(optarg);
				break;

			case 'o':
				o_outputFile = 1;
				filename = malloc(sizeof(optarg) + 1);
				filename = optarg;
				break;
			case '?':
				fprintf(stderr, "unknown option\n");
				exit(EXIT_FAILURE);
		}
	}
}


void print()
{
	printf("-h = %d\n", h_help);
	printf("-n = %d children = %d\n", n_maxChild, numChildren);
	printf("-s = %d allowed children = %d\n", s_allowedChildren, allowedChildren);
	printf("-b = %d number = %d\n", b_startNum, number);
	printf("-i = %d increment = %d\n", i_increment, incrementBy);
	printf("-o = %d filename = %s\n", o_outputFile, filename);
}


//==============================Memory Allocation=================================================

struct clock *clockMem(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, PERM);
	if(*shmid < 0)
	{
		perror("oss: ERROR: shmget (clock)");
		exit(1);
	}
	
	struct clock *temp = (struct clock *)shmat(*shmid, NULL, 0);

	if(temp == (void *)-1)
	{
		perror("oss: ERROR: shmat (clock)");
		exit(1);
	}

	return temp;
}

struct message *message(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, PERM);
	if(*shmid < 0)
	{
		perror("oss: ERROR: shmget (message)");
		exit(1);
	}

	struct message *temp = (struct message *)shmat(*shmid, NULL, 0);

	if(temp == (void *)-1)
	{
		perror("oss: ERROR: shmat (message)");
		exit(1);
	}
	return temp;
}

int *childResults(key_t key, size_t size, int *shmid)
{
	*shmid = shmget(key, size, PERM);
	if(*shmid < 0)
	{
		perror("oss: ERROR: shmget (childResults)");
		exit(1);
	}
	
	int *temp = (int *)shmat(*shmid, NULL, 0);
	if(temp == (void *)-1)
	{
		perror("oss: ERROR: shmat (childResults)");
		exit(1);
	}

	return temp;
}

void cleanUp(void *ptr, int id)
{

	if(shmdt(ptr) == -1)
	{
		perror("oss: ERROR: shmdt (cleanUp function)");
		exit(1);
	}

	if(shmctl(id, IPC_RMID, NULL) == -1)
	{
		perror("oss: ERROR: shmctl (cleanUp function)");
		exit(1);
	}
}

//===========================Signal Handling==============================================

void timesUp(int sig, siginfo_t *info, void *ptr)
{
	
	char msg[] = "Program times out at 2 seconds\n";
	int msgLen = sizeof(msg);
	write(STDERR_FILENO, msg, msgLen);

	printf("timesUp executed at %dnano %dsec\n\n", clockPtr->nano, clockPtr->sec);		
	shmdt((void *)clockPtr);
	shmdt((void *)msgPtr);
	shmdt((void *)arrPtr);
	shmctl(clockId, IPC_RMID, NULL);
	shmctl(msgId, IPC_RMID, NULL);
	shmctl(arrId, IPC_RMID, NULL);

	int i;
	for(i = 0; i < pidSize; i++)
	{
		if(pids[i] != 0)
		{
			if(kill(pids[i], SIGTERM) == -1)
				perror("oss: ERROR: SIGTERM (alarm)");
		}
	}
		
	free(pids);
	exit(0);
}

void userEnd(int signo, siginfo_t *info, void *ptr)
{
	char msg[] = "Caught CTRL + C while running\n\n";
	int len = sizeof(msg);
	write(STDERR_FILENO, msg, len);
	
	printf("caught it at %d nano %d sec\n\n", clockPtr->nano, clockPtr->sec);
	shmdt((void *)clockPtr);
	shmdt((void *)msgPtr);
	shmdt((void *)arrPtr);
	shmctl(clockId, IPC_RMID, NULL);
	shmctl(msgId, IPC_RMID, NULL);
	shmctl(arrId, IPC_RMID, NULL);

	int i;
	for(i = 0; i < pidSize; i++)
	{
		if(pids[i] != 0)
		{
			if(kill(pids[i], SIGTERM) == -1)
				perror("oss: ERROR: SIGTERM (ctrl+c)");
		}
	}

	free(pids);

	exit(0);
}

static int setAlarm()
{
	
	struct itimerval timer;
	timer.it_value.tv_sec = 2;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 0;
	return (setitimer(ITIMER_REAL, &timer, NULL));
}

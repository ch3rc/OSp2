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

key_t key = TESTKEY;
size_t clockSize = sizeof(struct clock *);
int clockId = 0;
struct clock *clockPtr = NULL;
struct shmid_ds test;
pid_t childPid = 0;

//============================================================================================

struct clock *clockMem(key_t key, size_t size, int *shmid);

void main(int argc, char *argv[])
{	
	initOpt();

	setOpt(argc, argv);

	if(h_help)
	{
		help();
	}
	
	printf("allocating memory for clock\n\n");

	clockPtr = clockMem(key, clockSize, &clockId);
	
	/*clockId = shmget(key, sizeof(struct clock *), PERM);
	if(clockId < 0)
	{
		perror("shmget");
		exit(1);
	}

	clockPtr = (struct clock *)shmat(clockId, NULL, 0);
	if(clockPtr == (void *)-1)
	{
		perror("shmat");
		exit(1);
	}*/
	
	clockPtr->status = START;
	
	childPid = fork();

	if(childPid < 0)
	{
		perror("fork");
		exit(1);
	}

	if(childPid > 0)
	{
		printf("child terminated regularly\n\n");
	}

	if(childPid == 0)
	{	char str[25];
		char id[25];
		snprintf(str, sizeof(str), "%d", 101);
		snprintf(id, sizeof(id), "%d", 1);
		execlp("./handleChild", str, id, NULL);
	}	

	while(clockPtr->status != RUN)
		sleep(1);
	
	printf("starting clock\n\n");
	
	printf("test line\n\n");
	
	while(1)
	{
	
		clockPtr->nano += 1;
		if(clockPtr->nano == 100000000)
		{
			printf("one second\n");
			clockPtr->sec += 1;
			clockPtr->nano = 0;
		}
		
		if(clockPtr->sec == 2)
		{
			printf("finished\n");
			break;
		}
	}
	
	while(clockPtr->status != FIN)	
		sleep(1);

	printf("detaching memory in oss.c\n\n");	
	
	if(shmdt((void *)clockPtr) == -1)
		perror("oss shmdt");

	printf("server detached memory\n\n");
	
	if(shmctl(clockId, IPC_RMID, &test) == -1)
		perror("shmctl");
		
	printf("memory removed in oss.c\n\n");

	printf("program exiting oss.c\n");
	

	print();
	
	exit(0);
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
	printf("-h = d\n", h_help);
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


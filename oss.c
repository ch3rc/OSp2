//==============================================================================
//Date:		February 14,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		oss.c
//==============================================================================

#include "oss.h"

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

int main(int argc, char *argv[])
{
	initOpt;
	
	setOpt(argc, argv);

	if(h_help)
	{
		help();
	}

	print();

	return 0;
}

//===========================================================================
//Date:		February 14,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		oss.h
//===========================================================================

#ifndef OSS_H
#define OSS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <sys/wait.h>

#define TESTKEY 0x12345678
#define START -1
#define RUN 0
#define FIN 1

extern int h_help;
extern int n_maxChild;
extern int s_allowedChildren;
extern int b_startNum;
extern int i_increment;
extern int o_outputFile;

struct clock{
	int status;
	int nano;
	int sec;
};

void initOpt();
void setOpt(int, char **);
void help();
void print();


#endif

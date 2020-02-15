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

extern int h_help;
extern int n_maxChild;
extern int s_allowedChildren;
extern int b_startNum;
extern int i_increment;
extern int o_outputFile;

void initOpt();
void setOpt(int, char **);
void help();
void print();


#endif

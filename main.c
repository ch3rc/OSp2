//===================================================================================
//Date:		February 14,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		main.c
//===================================================================================

#include "flags.h"

int main(int argc, char *argv[])
{

	initOpt();

	setOpt(argc, argv);

	if(h_help)
	{
		help();
	}

	print();

	return 0;
}

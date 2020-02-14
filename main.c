//===================================================================================
//Date:		February 14,2020
//Author:	Cody Hawkins
//Class:	Operating Systems (CS4760)
//Project:	Assignment 2
//File:		main.c
//===================================================================================


int main(int argc, char *argv[])
{

	initOpt();

	setOpt(argc, argv);

	if(h_help)
	{
		help();
	}

	return 0;
}

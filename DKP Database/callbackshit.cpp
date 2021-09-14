
#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<vector>
#include<sstream>
#include "sqlite3.h"
using namespace std;

extern bool printError;

//in shell.c - lines 3000 - 3003 : 
//static int callback(void *pArg, int nArg, char **azArg, char **azCol) {
//	/* since we don't have type info, call the shell_callback with a NULL value */
//	return shell_callback(pArg, nArg, azArg, azCol, NULL);
//}

int callbackshit(void *data, int argc, char **argv, char **azColName)
{
	int i;
	for (i = 0; i<argc; i++)
	{
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}

	printf("\n");


	return 0;

}


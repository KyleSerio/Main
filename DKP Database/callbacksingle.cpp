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
extern string globalString;

int callbacksingle(void *data, int argc, char **argv, char **azColName)
{

	globalString = argv[0];
	std::cout << argv[0];
	
	printError = false;

	return 0;
}


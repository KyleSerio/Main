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
extern string globalCommand[1000];
extern int globalIndex;

//Used to retract the dkp given from deleted raids, no printing done
//called by -
//command = "SELECT PLAYER,DKP FROM RAIDDUMP WHERE FILE like '%RaidRoster-20171010%';";
//sql = &command[0u];
//check = sqlite3_exec(db, sql, callbackremovedump, 0, &zErrMsg);

int callbackremovedump(void *data, int argc, char **argv, char **azColName)
{
	string result;
	printError = true;
	string temp;


	//0 is name
	//1 is dkp
	result = "UPDATE PLAYER SET DKP = DKP - ";
	temp = argv[1];
	result += temp;
	temp = argv[0];
	result += " WHERE NAME = '" + temp + "';";

	if (globalIndex > 997)
	{
		globalIndex = 0;
		std::cout << "globalCommand over 999!!" << endl;
	}

	globalCommand[globalIndex] = result;
	globalIndex++;
	globalCommand[globalIndex] = "UPDATE PLAYER SET CHECKS = CHECKS - 1, CHECKSTOTAL = CHECKSTOTAL - 1 WHERE NAME = '" + temp + "';";
	globalIndex++;

	printError = false;

	return 0;
}


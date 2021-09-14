#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<list>
#include "header.h"
using namespace std;
extern bool printError;

bool exists(string table, string column, string value,sqlite3* db, int print)
{
	char *zErrMsg = 0;
	string command;
	char* sql;

	printError = true;

	command = "SELECT * FROM " + table + " WHERE " + column + " = '" + value + "';";
	sql = &command[0u];
	if (print == 1)
	{
		int check = sqlite3_exec(db, sql, callbacksingle, 0, &zErrMsg);
	}
	else
	{
		int check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
	}
	

	return !printError;
}

bool exists(string table, string column, int value, sqlite3* db, int print)
{
	char *zErrMsg = 0;
	string command;
	char* sql;

	printError = true;

	command = "SELECT * FROM " + table + " WHERE " + column + " = " + to_string(value) + ";";
	sql = &command[0u];

	if (print == 1)
	{
		int check = sqlite3_exec(db, sql, callbacksingle, 0, &zErrMsg);
	}
	else
	{
		int check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
	}

	return !printError;
}
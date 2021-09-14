#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<list>
#include "header.h"
using namespace std;
extern bool printError;
extern string globalString;

string findDBVal(string value, string table, string column, string known, sqlite3* db)
{
	char *zErrMsg = 0;
	string command;
	char* sql;

	printError = true;

	command = "SELECT " + value + " FROM " + table + " WHERE " + column + " = '" + known + "';";
	sql = &command[0u];
	int check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

	return globalString;
}

string findDBVal(string value, string table, string column, int known, sqlite3* db)
{
	char *zErrMsg = 0;
	string command;
	char* sql;

	printError = true;

	command = "SELECT " + value + " FROM " + table + " WHERE " + column + " = " + to_string(known) + ";";
	sql = &command[0u];
	int check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

	return globalString;
}
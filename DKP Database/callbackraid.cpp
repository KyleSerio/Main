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

int callbackraid(void *data, int argc, char **argv, char **azColName)
{
	int index;
	int colWidth;
	int dataWidth = 15;
	int longest = 0;
	printError = true;
	string temp;
	//Column Widths = 0,  1,  2,  3, 4   5   6  = 84 total
	int column[10] = { 13, 6, 11, 8, 13, 10, 11};


	//sql = "CREATE TABLE RAID(DATE INT UNIQUE,ITEMS INT,DKPEARNED INT,RAIDERS INT,RAIDERSAVG FLOAT, DKPSPENT INT, DKPRAID INT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	cout << "|| ";

	for (index = 0; index < argc; index++)
	{

		string temp;

		if (argv[index])
		{
			temp = argv[index];

			if (temp.length() < column[index])
			{

				if (index == 0)
				{
					string dateForm;

					if (temp.length() < 8)
					{
						temp = "0" + temp;
					}

					dateForm = temp.substr(4,2) + "/" + temp.substr(6,2) + "/" + temp.substr(0,4);

					temp = dateForm;
				}


				colWidth = column[index] - temp.length();
				string spaces(colWidth, ' ');
				temp += spaces;
			}
			else
			{
				if (index != 0)
				{
					temp = temp.substr(0, column[index] - 2);
					temp += "..";
				}
				else
				{
					temp = "DATE ERR";
				}
			}


		}
		else
		{
			temp = "NULL";
			string spaces;
			if (column[index] > temp.length())
			{
				colWidth = column[index] - temp.length();
				spaces.assign(colWidth, ' ');
			}
			else
			{
				colWidth = column[index];
				spaces.assign(colWidth, ' ');
			}

			temp += spaces;
		}

		if (index < 7)
		{
			if (index + 2 < argc)
			{

				cout << temp << "| ";
			}
			else
			{
				cout << temp;
			}
		}


	}



	cout << "||" << endl;
	printError = false;

	return 0;
}


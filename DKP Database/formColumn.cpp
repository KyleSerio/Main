#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;

extern string globalString;

string formColumn(string word, int columnSize)
{
	string result;
	string spaces;

	if (word.length() < columnSize)
	{
		spaces.assign(columnSize - word.length(), ' ');
		result = word + spaces;
	}
	else
	{
		result = word.substr(0, columnSize - 3);
		result += "...";
	}


	return result;
}

string formColumn(int &dkpVal, int columnSize)
{
	string result;
	string spaces;
	int dkpTest = dkpVal;
	int dkpDigits = 0;



	if (dkpVal >= 0 && dkpVal < 10000)
	{
		if (globalString == "Box")
		{
			while (dkpTest > 0)
			{
				dkpTest = dkpTest / 10;
				dkpDigits++;
			}

			dkpVal = dkpVal * 3;
			dkpTest = dkpVal;
			while (dkpTest > 0)
			{
				dkpTest = dkpTest / 10;
				dkpDigits++;
			}
			spaces.assign(columnSize - (dkpDigits + 3), ' ');
			result = to_string((dkpVal / 3)) + "(" + to_string(dkpVal) + ")" + spaces;
		}
		else
		{
			if (dkpTest == 0)
			{
				dkpDigits = 1;
			}
			else
			{
				while (dkpTest > 0)
				{
					dkpTest = dkpTest / 10;
					dkpDigits++;
				}
			}
			spaces.assign(columnSize - dkpDigits, ' ');
			result = to_string(dkpVal) + spaces;
		}
	}


	return result;
}
#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<list>
#include "header.h"
using namespace std;

string findWord(string stringLine, int &place) 
{
	string word = "";
	if (place >= stringLine.length())
	{
		return "Error: String subscript out of range";
	}

	if (stringLine[place] == ' ')
	{
		place++;
	}

	while (place < stringLine.length() && stringLine[place] != ' ')
	{
		word += tolower(stringLine[place]);
		place++;
	}

	if (place < stringLine.length())
	{
		place++;
	}

	return word;
}


string findWord(string stringLine, int &place,bool changeCase)
{
	string word = "";
	if (place >= stringLine.length())
	{
		return "Error: String subscript out of range";
	}

	if (stringLine[place] == ' ')
	{
		place++;
	}

	while (place < stringLine.length() && stringLine[place] != ' ')
	{
		word += stringLine[place];
		place++;
	}

	if (place < stringLine.length())
	{
		place++;
	}

	return word;
}

string peekWord(string stringLine, int place)
{
	string word = "";
	if (place >= stringLine.length())
	{
		return "Error: String subscript out of range";
	}

	if (stringLine[place] == ' ')
	{
		place++;
	}

	while (place < stringLine.length() && stringLine[place] != ' ')
	{
		word += tolower(stringLine[place]);
		place++;
	}

	if (place < stringLine.length())
	{
		place++;
	}

	return word;
}

string peekWord(string stringLine, int place, bool changeCase)
{
	string word = "";
	if (place >= stringLine.length())
	{
		return "Error: String subscript out of range";
	}

	if (stringLine[place] == ' ')
	{
		place++;
	}

	while (place < stringLine.length() && stringLine[place] != ' ')
	{
		word += stringLine[place];
		place++;
	}

	if (place < stringLine.length())
	{
		place++;
	}

	return word;
}



string findItem(string stringLine, int &place)
{
	if (place >= stringLine.length())
	{
		return "Error: String subscript out of range";
	}

	string word = "";
	string temp;

	if (stringLine[place] == ' ')
	{
		place++;
	}

	word[0] = toupper(stringLine[place]);
	place++;


	while (!isdigit(stringLine[place]))
	{
		if (stringLine[place - 1] == ' ')
		{
			temp = peekWord(stringLine,place);
			if (temp != "of" && temp != "the" && temp != "and" && temp != "for")
			{
				word += toupper(stringLine[place]);
			}
			else
			{
				word += tolower(stringLine[place]);
			}
		}
		else if (stringLine[place - 1] == '-' && isalpha(stringLine[place]))
		{
			word += toupper(stringLine[place]);
		}
		else
		{
			word += tolower(stringLine[place]);
		}

		
		place++;
	}


	while (word[word.length()] == ' ')
	{
		word = word.substr(0, word.length() - 1);
	}

	return word;
}
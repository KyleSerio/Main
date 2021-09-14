#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;

string getItem()
{
	string itemName;
	string temp;
	bool pass = true;
	char test = '~';
	int index2 = 0;
	int placeHolder = 0;

	std::getline(std::cin, itemName);
	std::cout << endl;

	



	if (!itemName.empty() && isalpha(itemName[0]))
	{
		while (pass && index2 < itemName.length())
		{
			if (!isalpha(itemName[index2]) && itemName[index2] != '\'' && itemName[index2] != '-' && itemName[index2] != ' ')
			{
				pass = false;
			}
			index2++;
		}

		while (!pass)
		{
			std::cout << "[Enter] to skip: Input not valid, retry: ";
			std::getline(std::cin, itemName);
			std::cout << endl;
			index2 = 0;
			pass = true;
			while (pass && index2 < itemName.length())
			{
				if (!isalpha(itemName[index2]) && itemName[index2] != '\'' && itemName[index2] != '-' && itemName[index2] != ' ')
				{
					pass = false;
				}
				index2++;
			}
		}

		if (!itemName.empty())
		{
			itemName[0] = toupper(itemName[0]);

			for (int index2 = 1; index2 < itemName.length(); index2++)
			{
				test = itemName[index2];

				if (test == ' ' || test == '-' || test == '\'')
				{
					if (itemName[index2 + 1] == 'o' && itemName[index2 + 2] == 'f' && itemName[index2 + 3] == ' ')
					{
						itemName[index2 + 1] = tolower(itemName[index2 + 1]);
					}
					else if (itemName[index2 + 1] == 't' && itemName[index2 + 2] == 'h' && itemName[index2 + 3] == 'e' && itemName[index2 + 4] == ' ')
					{
						itemName[index2 + 1] = tolower(itemName[index2 + 1]);
					}
					else if (itemName[index2 + 1] == 'a' && itemName[index2 + 2] == 'n' && itemName[index2 + 3] == 'd' && itemName[index2 + 4] == ' ')
					{
						itemName[index2 + 1] = tolower(itemName[index2 + 1]);
					}
					else if (itemName[index2 + 1] == 'f' && itemName[index2 + 2] == 'o' && itemName[index2 + 3] == 'r' && itemName[index2 + 4] == ' ')
					{
						itemName[index2 + 1] = tolower(itemName[index2 + 1]);
					}
					else if (test == '\'' && itemName[index2 + 1] == 's' && itemName[index2 + 2] == ' ')
					{
						itemName[index2 + 1] = tolower(itemName[index2 + 1]);
					}
					else
					{
						itemName[index2 + 1] = toupper(itemName[index2 + 1]);
						index2++;
					}
				}
				else
				{
					itemName[index2] = tolower(itemName[index2]);
				}

			}
		}

	}

	return itemName;
}


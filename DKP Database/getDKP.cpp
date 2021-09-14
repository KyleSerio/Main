#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;

bool isNum(string test);

int getDKP()
{
	string input;
	int num = 0;

	std::getline(std::cin,input);

	if (input.empty())
	{
		return -1;
	}


	while (!input.empty())
	{
		if (isNum(input))
		{
			if (input.length() > 5)
			{
				std::cout << "Value too large!!" << endl;
			}
			else
			{
				num = stoi(input);		
			}

			
		}
		else 
		{
			std::cout << "[Enter] to skip: Enter an integer ";
			std::getline(std::cin, input);
			std::cout << endl;
		}

		if (num < 0 || num > 2500 && isNum(input))
		{
			std::cout << "[Enter] to skip: Enter a valid DKP amount (0 - 2500): ";
			std::getline(std::cin, input);
			std::cout << endl;
		}
		else if (num >= 0 && num <= 2500 && isNum(input))
		{
			return num;
		}

	}


	return -1;
}
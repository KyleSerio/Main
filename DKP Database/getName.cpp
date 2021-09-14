#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;

string getName()
{
	string input;
	bool pass = true;

	std::getline(std::cin, input);

	while (input.length() > 15 && !input.empty())
	{
		std::cout << "[Enter] to skip: Name is too long!";
		std::getline(std::cin, input);
	}
	
	if (!input.empty())
	{
		for (int index = 0; index < input.length(); index++)
		{
			if (!isalpha(input[index]))
			{
				pass = false;
			}
			else
			{
				input[index] = tolower(input[index]);
			}
		}

		while (!pass)
		{
			std::cout << "[Enter] to skip: Non-Alpha character found, please enter a valid one word name!";
			std::getline(std::cin, input);
			pass = true;

			if (!input.empty())
			{
				for (int index = 0; index < input.length(); index++)
				{
					if (!isalpha(input[index]))
					{
						pass = false;
					}
					else
					{
						input[index] = tolower(input[index]);
					}
				}
			}

		}


		input[0] = toupper(input[0]);
			
	}
	

	return input;
}
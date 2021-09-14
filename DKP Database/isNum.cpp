#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;

bool isNum(string test)
{
	bool digits = true;
	int index = 0;

	while (digits && index < test.length())
	{
		if (!isdigit(test[index]))
		{
			digits = false;
		}
		index++;
	}

	return digits;
}
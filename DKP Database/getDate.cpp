#pragma once
#include<iostream>
#include<string>
#include "header.h"
using namespace std;


int getDate(string &dateForm)
{
	int dateNum = 0;
	int year = 0;
	int month = 0;
	int day = 0;
	string temp;

	std::getline(std::cin,dateForm);
	std::cout << endl;


	while (!dateForm.empty() && dateForm.length() != 8 && !(dateForm[0] == '0' && dateForm.length() == 7))
	{
		std::cout << "[Enter] to skip: Incorrect date Format! (MMDDYYYY) ";
		std::getline(std::cin, dateForm);
		std::cout << endl;
	}

	if (dateForm.empty())
	{
		return -1;
	}
	// 0 1 2 3 4 5 6 7	
	// Y Y Y Y M M D D
	// 1 0 0 0 0 0 0 0
	//   1 0 0 0 0 0 0
	//     1 0 0 0 0 0
	//       1 0 0 0 0
	//         1 0 0 0
	//           1 0 0
	//             1 0
	//               1


	dateNum =  ((dateForm[4] - 48) * 10000000) + ((dateForm[5] - 48) * 1000000) + ((dateForm[6] - 48) * 100000) + ((dateForm[7] - 48) * 10000);
	dateNum += ((dateForm[0] - 48) * 1000)     + ((dateForm[1] - 48) * 100);
	dateNum += ((dateForm[2] - 48) * 10)       + ((dateForm[3] - 48) * 1);


	temp = dateForm.substr(0,2) + "/" + dateForm.substr(2, 2) + "/" + dateForm.substr(4, 4);
	dateForm = temp;

	return dateNum;
}
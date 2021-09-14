#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<list>
#include "header.h"
using namespace std;

//struct purchase
//{
//	string logLine;
//	string itemName;
//	string buyerName;
//	string buyerType;
//	int dkpVal;
//};

purchase parseLootLine(string lootLine,sqlite3 *db)
{
	purchase result;

	string temp;
	string possiblePlayer;
	string store;
	bool playerFound = false;
	bool itemFound = false;
	bool dkpFound = false;
	bool upper = false;
	bool grats = false;
	bool multiple = false;
	int index = 0;
	int check = 0;

	string table  = "PLAYER";
	string column = "NAME";
	string value = "";


	//Find start of player /rsay, then add the remaining to result.logLine
	while (lootLine[index] != '\'' && index < lootLine.length())
	{
		index++;
	}
	index++; //increment to first letter of their text
	check = index;
	while (check < lootLine.length() && lootLine[check] != '\'')
	{
		result.logLine += lootLine[check];
		check++;
	}
	check = 0;

	if (index < lootLine.length())
	{

		while (index < lootLine.length() && !dkpFound)
		{
			value = peekWord(lootLine, index, 0);
			temp = peekWord(lootLine, index);

			if (isupper(value[0]) && temp != "grats" && temp != "gratz")
			{
				if (temp.substr(0, 4) == "grat" && lootLine[index + 4] == ' ')
				{
					upper = false;
					grats = true;
				}
				else
				{
					upper = true;
				}
			}
			else if (temp == "grats" || temp == "gratz" || (temp == "grat" && lootLine[index + 4] == ' '))
			{
				grats = true;
				upper = false;
			}
			else
			{
				upper = false;
			}


			if (grats)
			{
				grats = false;
				upper = false;
				index += temp.length() + 1;
				temp = peekWord(lootLine, index, 0);

				if (exists(table, column, temp, db,0))
				{
					if (temp == "rot")
					{
						result.buyerName = "Rot";
						result.buyerType = "Main";
						index += temp.length() + 1;
					}
					else
					{
						result.buyerName = temp;
						result.buyerType = findDBVal("TYPE", table, column, temp, db);
						index += temp.length() + 1;
					}
					playerFound = true;
				}
				else if (isupper(temp[0]))
				{
					possiblePlayer = temp;
					index += temp.length() + 1;
				}
			}
			else if (isdigit(temp[0]) && !dkpFound) //indicates the dkp value
			{
				temp = findWord(lootLine, index);

				result.dkpVal = stoi(temp);
				std::cout << result.dkpVal << endl;
				dkpFound = true;
			}
			else if (!playerFound && (exists(table, column, value, db,0) || temp == "rot"))
			{
				if (temp == "rot")
				{
					result.buyerName = "Rot";
					result.buyerType = findDBVal("TYPE", table, column, "Rot", db);
					index += temp.length() + 1;
				}
				else
				{
					result.buyerName = value;
					result.buyerType = findDBVal("TYPE", table, column, value, db);
					index += temp.length() + 1;
				}
				playerFound = true;
			}
			else if (upper && !itemFound)
			{


				while (upper && index < lootLine.length())
				{
					temp = peekWord(lootLine, index, 0);
					store = peekWord(lootLine, index);
					string firstCap;
					firstCap = store;
					firstCap[0] = toupper(firstCap[0]);

					for (int index2 = 0; index2 < temp.length(); index2++)
					{
						if (!isalpha(temp[index2]) && temp[index2] != ' ' && temp[index2] != '-' && temp[index2] != '\'')
						{
							upper = false;
						}
						else if (temp[index2] == '-' || temp[index2] == '\'')
						{
							if (temp[index2 + 1] == '-' || temp[index2 + 1] == '\'')
							{
								temp = temp.substr(0,temp.length() - 2);
							}
						}
					}

					if (upper)
					{
						if (store == "grats" || store == "gratz" || (store == "grat" && lootLine[index + 4] == ' '))
						{
							upper = false;
						}
						else if (temp != firstCap && temp != "of" && temp != "the")
						{

							check = 0;
							while (check < firstCap.length())
							{
								if ((firstCap[check] == '-' || firstCap[check] == '\'') && isalpha(firstCap[check + 1]) && firstCap[check + 1] != ' ' && firstCap[check + 1 != '-'])
								{
									firstCap[check + 1] = toupper(firstCap[check + 1]);
								}
								check++;
							}

							if (temp == firstCap)
							{

								temp = findWord(lootLine, index, 0);

								if (result.itemName == "")
								{
									result.itemName = temp;
								}
								else
								{
									result.itemName += ' ' + temp;
								}
							}
							else
							{
								upper = false;
							}

						}
						else if (!playerFound && exists(table, firstCap, temp, db,0))
						{
							upper = false;
						}
						else
						{
							temp = findWord(lootLine, index, 0);

							if (result.itemName == "")
							{
								result.itemName = temp;
							}
							else
							{
								result.itemName += ' ' + temp;
							}
						}

					}
				}

				itemFound = true;

			}
			else if (playerFound && !dkpFound && (temp == "and" || temp == "x2"))
			{
				multiple = true;
				index += temp.length() + 1;

				if (temp == "and")
				{	
					temp = findWord(lootLine,index);
					temp[0] = toupper(temp[0]);
					result.secondBuyer = temp;

					result.errorFlag = "two winners";

				}
				else if (temp == "x2")
				{
					result.errorFlag = "x2";
				}


			}
			else
			{
				index += temp.length() + 1;
			}
		}

		//Setting error flags
		if (!playerFound)
		{

			result.buyerName = possiblePlayer;
			result.errorFlag = "player";
		}
		else if (!itemFound)
		{
			result.errorFlag = "item";
		}
		else if (!dkpFound)
		{
			result.errorFlag = "dkp";
		}
		else
		{
			result.complete = true;
		}
	}

	return result;
}
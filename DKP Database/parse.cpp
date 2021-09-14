#pragma once
#include "raidDump.h"
#include "header.h"
#include "database.h"


void parse(string fileName, database& mainDB, int date) //runs once for each file/dump given to it
{
	string flag;
	string charName;
	int charLevel;
	int groupNum;
	string charClass;




	ifstream readFrom;
	readFrom.open(fileName);

	mainDB.newDump();

	while (!readFrom.eof())
	{
		readFrom >> groupNum;
		readFrom >> charName;
		readFrom >> charLevel;
		readFrom >> charClass;
		readFrom >> flag;

		while (flag != "Yes" && flag != "No")
		{
			readFrom >> flag;
		}

		//mainDB.addPlayer();

	}

	cout << mainDB.raidDumps() << " -> ";
	mainDB.incDumpCount();
	cout << mainDB.raidDumps() << endl;

	readFrom.close();
}

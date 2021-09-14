#include "database.h"

extern bool printError;
extern string globalCommand[1000];
extern int globalIndex;
extern string globalString;
extern int globalInt;

void database::update() //Used as a one-time when manually updating the database
{
	string command;
	string test = "~";


}


void database::mainMenu()
{
	string input = "~";
	
	while (!input.empty()) //Outer menu - displays player list, and asks for a selection
	{
		std::cout << "Current Entries: " << endl;
		std::cout << "Players   : " << playerCount << endl;
		std::cout << "Items     : " << itemCount << endl;
		std::cout << "Raids     : " << raidCount << endl;

		std::cout << "Main Menu: [Enter] to Quit Program) Work with [P]layers, [I]tems, [R]aids or [A]dmin Menu? : ";
		std::getline(std::cin,input);
		std::cout << endl;

		
		if (!input.empty())
		{
			input = toupper(input[0]);

			if (input[0] == 'P')
			{
				accessPlayer();
			}
			else if (input[0] == 'I')
			{
				accessItem();
			}
			else if (input[0] == 'R')
			{
				accessRaid();
			}
			else if (input[0] == 'A')
			{
				accessAdmin();
			}
			else
			{
				std::cout << "Error: No Such Option " << endl << endl;
			}
		}



	}//end while - outer selection
}


void database::accessItem()
{
	string inputItem = "~";


	while (!inputItem.empty()) //Outer menu - displays player list, and asks for a selection
	{
		printItems();


		std::cout << "Item Menu: [Enter] to Quit Program) Would you like to [V]iew, [U]pdate, [A]dd, or [R]emove? : ";
		std::getline(std::cin, inputItem);
		std::cout << endl;

		if (inputItem.empty())
		{
			std::cout << "Leaving..." << endl << endl;
		}
		else
		{
			inputItem[0] = toupper(inputItem[0]);

			if (inputItem[0] == 'U')
			{
				updateItem();
			}
			else if (inputItem[0] == 'A')
			{
				addItem();
			}
			else if (inputItem[0] == 'R')
			{
				removeItem();
			}
			else if (inputItem[0] == 'V')
			{
				viewItem();
			}
			else
			{
				std::cout << "Error: No Such Option " << endl << endl;
			}
		}



	}//end while - outer selection
}

void database::accessAdmin()
{
	string command = "";
	string input = "";
	string password = "";

	std::cout << "Admin Menu: [Enter] to Leave) [W]ipe Raid Data? : ";
	std::getline(std::cin, input);
	std::cout << endl;

	if (!input.empty())
	{
		input = toupper(input[0]);

		if (input[0] == 'W')
		{
			std::cout << "Enter the Password to wipe database : ";
			std::getline(std::cin, password);
			std::wcout << endl;

			if (password == "admin123")
			{
				command = "DROP TABLE FILECHECK;";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				command = "CREATE TABLE FILECHECK(FILE TEXT UNIQUE);";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				command = "DROP TABLE RAID;";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				sql = "CREATE TABLE RAID(DATE INT UNIQUE,ITEMS INT,DKPEARNED INT,RAIDERS INT,RAIDERSAVG FLOAT, DKPSPENT INT, DKPRAID INT,DUMPS INT);";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

				sql = "DELETE FROM RAIDDUMP;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
				sql = "VACUUM;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

				sql = "DELETE FROM ITEM;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
				sql = "VACUUM;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

				sql = "UPDATE PLAYER SET DKP = 0, DKPTOTAL = 0, ATTENDANCE = 0, LIFETIME = 0, CHECKS = 0, CHECKSTOTAL = 0;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
				sql = "VACUUM;";
				check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

				std::cout << "Database has been Wiped!! " << endl;
			}
			else
			{
				std::cout << "Incorrect Password! " << endl;
			}
			password = "";

		}
		else
		{
			std::cout << "No such Option!" << endl;
		}

	}


}

void database::accessPlayer()
{
	string input = "~";

	while (!input.empty()) //Outer menu - displays player list, and asks for a selection
	{
		printPlayers();

		std::cout << "Player Menu: [Enter] to Main Menu) Would you like to [V]iew, [U]pdate, [A]dd, or [R]emove? : ";
		std::getline(std::cin, input);
		
		std::cout << endl;

		if (input.empty())
		{

		}
		else
		{
			input[0] = toupper(input[0]);

			if (input[0] == 'U')
			{
				updatePlayer();
			}
			else if (input[0] == 'A')
			{
				addPlayer();
			}
			else if (input[0] == 'R')
			{
				removePlayer();
			}
			else if (input[0] == 'V')
			{
				viewPlayer();
			}
			else
			{
				std::cout << "Error: No Such Option " << endl << endl;
			}
		}




	}//end while - outer selection

}

void database::accessRaid()
{
	string input = "~";


	while (!input.empty()) //Outer menu - displays player list, and asks for a selection
	{
		printRaids();


		std::cout << "Raid Menu: [Enter] for Main Menu) Would you like to View A [R]aid, View [D]umps, Add [F]iles, or [W]ipe Raids? : ";
		std::getline(std::cin, input);
		std::cout << endl;

		if (input.empty())
		{

		}
		else
		{
			input = toupper(input[0]);


		    if (input[0] == 'F')
			{
				addFile();
			}
			else if (input[0] == 'R')
			{
				viewRaid();
			}
			else if (input[0] == 'W')
			{
				removeRaid();
			}
			else if (input[0] == 'D')
			{
				viewDumps();
			}
			else
			{
				std::cout << "Error: No Such Option " << endl << endl;
			}

		}


	}//end while - outer selection
}



void database::addItem()
{
	string command;
	const unsigned char* testResult;
	string itemName = "";
	string buyer = "";
	bool leave = false;
	int date = 0;
	int dkp = -100;

	//sql = "INSERT INTO ITEM(ID,DATE,BUYER,ITEM,SPENT) VALUES (1, 12202017, 'Venalin', 'Morguecaller', 50);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

	printError = true;

	std::cout << "Adding Item: [Enter] for Main Menu) What is the name of the item?: ";
	itemName = getItem();

	std::cout << "Item name : " << itemName << endl;

	if (!itemName.empty())
	{

		while (printError && !leave)
		{

			std::cout << "[Enter] to exit: Who bought this item?: ";
			buyer = getName();

			if (buyer.empty())
			{
				std::cout << "Skipping..." << endl;
				leave = true;
			}
			else
			{
				printError = true;
				std::cout << "Adding to ";
				std::cout << endl;

				if (!exists("PLAYER", "NAME", buyer, db, 1))
				{
					std::cout << "Error: Invalid player" << endl;
				}
				std::cout << endl;
			}

		}

		while ((dkp < 0 || dkp > 2500) && !leave)
		{
			std::cout << "[-1] to exit: How much did " + buyer + " pay?: ";
			dkp = getDKP();


			if (dkp == -1)
			{
				leave = true;
			}

		}

		date = 0;
		while (date != -1 && !leave)
		{
			string testDate;
			std::cout << "[-1] to exit) Enter New Raid Date(MMDDYYYY): ";
			date = getDate(testDate);

			if (date != -1)
			{
				command = "INSERT INTO ITEM(ID,DATE,BUYER,ITEM,SPENT) VALUES (?, ?, ?, ?, ?);";
				sqlC = &command[0u];
				check = sqlite3_prepare(db, sqlC, strlen(sqlC), &stmt, &cErrMsg);

				if (check == SQLITE_OK)
				{
					arg1 = buyer.c_str();
					arg2 = itemName.c_str();

					sqlite3_bind_int(stmt, 1, itemCount);
					sqlite3_bind_int(stmt, 2, date);
					sqlite3_bind_text(stmt, 3, arg1, strlen(arg1), 0);
					sqlite3_bind_text(stmt, 4, arg2, strlen(arg2), 0);
					sqlite3_bind_int(stmt, 5, dkp);

					sqlite3_step(stmt);
					sqlite3_finalize(stmt);
				}

				incItemCount();

				std::cout << "Added " << buyer << "'s purchase to the database! " << endl;
				printPlayerItems(buyer);
				date = -1;				
			}
		}

	}

	std::cout << endl;
	
}

void database::addItem(int date, string buyerName, string itemName, int dkpVal)
{
	string command;

	incItemCount();
	command = "INSERT INTO ITEM(ID,DATE,BUYER,ITEM,SPENT) VALUES (" + to_string(itemCount) + ", " + to_string(date) + ", '" + buyerName + "', '" + itemName + "', " + to_string(dkpVal) + ");";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

	command = "UPDATE PLAYER SET DKP = DKP - " + to_string(dkpVal) + " WHERE NAME = '" + buyerName + "';";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
}

bool database::addPlayer(string newName, string newClass, int newLevel)
{
	string command;
	string type;
	bool success;




	std::cout << "Adding " << newName << " the level " << newLevel << " " << newClass << "..." << endl;


	std::cout << "[Enter] to cancel: What type of player is " + newName + "? (Main/Box etc.): ";
	type = getName();

	if (type.empty())
	{

	}
	else
	{
		command = "INSERT INTO PLAYER(name,level,class,type,dkp,dkptotal,attendance,lifetime,checks,checkstotal) " \
			"VALUES ('" + newName + "', " + to_string(newLevel) + ",'" + newClass + "', '" + type + "', 0, 0, 0, 0, 0, 0);";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
		incPlayerCount();
		printError = true;

		command = "SELECT * FROM PLAYER WHERE NAME = '" + newName + "';";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

		if (printError)
		{
			std::cout << "Print Error!" << endl;
			success = false;
		}
		else
		{
			std::cout << "Quick Added " << newName << " to the database! " << endl << endl;
			success = true;
		}



	}
	std::cout << endl;

	return success;

}

void database::addPlayer(string newName) //Takes input from user to add a player to PLAYER table
{
	string command;
	string type;
	bool cancel = false;
	int level = 0;
	string newClass;



		std::cout << "Adding " << newName << " to the database..." << endl;


		newName[0] = toupper(newName[0]);
		printError = true;
		command = "SELECT * FROM PLAYER WHERE NAME = '" + newName + "';";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

		if (!printError)
		{
			std::cout << "Error: Player already exists!" << endl;
			std::cout << "Exiting..." << endl;
		}
		else
		{
			while (level != -1 && !cancel)
			{
				std::cout << "[-1] to exit) What level is " + newName + "?: ";
				level = getDKP();

				if (level != -1)
				{
					if (level < 1 || level > 110)
					{
						std::cout << "Error: Invalid level" << endl;
						level = 1;
					}
					else
					{
						std::cout << "[Enter] to Cancel: What class is " + newName + "?: ";
						newClass = getName();

						if (!newClass.empty() && !cancel)
						{

							std::cout << "[Enter] to Cancel: What type of player is " + newName + "? (Main/Box etc.): ";
							type = getName();

							if (!type.empty() && !cancel)
							{
								command = "INSERT INTO PLAYER(name,level,class,type,dkp,dkptotal,attendance,lifetime,checks,checkstotal) " \
									"VALUES ('" + newName + "', " + to_string(level) + ",'" + newClass + "', '" + type + "', 0, 0, 0, 0, 0, 0);";
								sql = &command[0u];
								check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
								level = -1;
								incPlayerCount();
								std::cout << "Added " << newName << " to the database! " << endl;

								printPlayers();
							}
							else
							{
								cancel = true;
							}
						}
						else
						{
							cancel = true;
						}
					}
				}
				else
				{
					cancel = true;
				}
			}
			
		}

		std::cout << endl;

	

}

void database::addPlayer() //Takes input from user to add a player to PLAYER table
{
	string command;
	string name = "~";
	string type;
	bool cancel;
	string newClass;


	while (!name.empty()) //Outer Loop Exit Condition
	{
		int level = 0;
		cancel = false;
		std::cout << "[Enter] for Main Menu) Who would you like to add?: ";
		name = getName();

		
		if (!name.empty()) 
		{
			printError = true;
			command = "SELECT * FROM PLAYER WHERE NAME = '" + name + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			if (printError)
			{
				while (level != -1 && !cancel)
				{
					std::cout << "[-1] to exit) What level is " + name + "?: ";
					level = getDKP();

					if (level != -1)
					{
						if (level < 1 || level > 110)
						{
							std::cout << "Error: Invalid level" << endl;
							level = 1;
						}
						else
						{
							std::cout << "[Enter] to Cancel: What class is " + name + "?: ";
							newClass = getName();

							if (!newClass.empty() && !cancel)
							{
								std::cout << "[Enter] to Cancel: What type of player is " + name + "? (Main/Box etc.): ";
								type = getName();

								if (!type.empty() && !cancel)
								{

									command = "INSERT INTO PLAYER(name,level,class,type,dkp,dkptotal,attendance,lifetime,checks,checkstotal) " \
										"VALUES ('" + name + "', " + to_string(level) + ",'" + newClass + "', '" + type + "', 0, 0, 0, 0, 0, 0);";
									sql = &command[0u];
									check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
									level = -1;
									incPlayerCount();
									std::cout << "Added " << name << " to the database! " << endl;

									printPlayers();
								}
								else
								{
									cancel = true;
								}


							}
							else
							{
								cancel = true;
							}


						}
					}
					else
					{
						cancel = true;
					}

				}
			}
			else
			{
				std::cout << "Error: Player already exists!" << endl;
			}


		}

		std::cout << endl;

	}


}

void database::addFile()
{
	string input;

	char filename[500];
	string file[100];
	int index = 0;
	string test;
	OPENFILENAME ofn;
	ZeroMemory(&filename, sizeof(filename));
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
	ofn.lpstrFilter = "Text Files\0*.txt\0Any File\0*.*\0";
	ofn.lpstrFile = filename;
	ofn.nMaxFile = 500;
	ofn.lpstrTitle = "Select a File, yo!";
	ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_EXPLORER;




	std::cout << "[Enter] to Cancel: Would you like to add [D]umps or [L]oots? : ";
	std::getline(std::cin,input);
	std::cout << endl;



	if (input.empty())
	{

	}
	else
	{
		if (GetOpenFileNameA(&ofn))
		{
			
			char* str = ofn.lpstrFile;
			string directory = str;

			str += (directory.length() + 1);
			directory += '\\';

			while (*str) 
			{
				string filenamestr = str;
				str += (filenamestr.length() + 1);
				// use the filename, e.g. add it to a vector

				file[index] = filenamestr;


				std::cout << index << "] (MULT) You chose the file \"" << file[index] << "\"\n";
				

				index++;

			}

			if (index == 0) //index is zero if all data is contained in the directory!
			{
				file[index] = directory;
				index = 1;
				std::cout << index << "] (SINGLE)You chose:     " << directory << endl;
			}

			input = toupper(input[0]);

			if (input[0] == 'D')
			{
				parseDump(directory, file, index);
			}
			else if (input[0] == 'L')
			{
				parseLoots(directory, file, index);
			}
		}
		else
		{
			// All this stuff below is to tell you exactly how you messed up above. 
			// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
			switch (CommDlgExtendedError())
			{
			case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
			case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
			case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
			case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
			case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
			case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
			case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
			case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
			case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
			case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
			case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
			case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
			case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
			case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
			case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
			default: std::cout << "You cancelled.\n";
			}

		}
	}
		
} 




void database::removeItem()
{
	string command;
	int id = 0;

	while (id != -1)
	{

		std::cout << "Remove Menu: [-1] for Main Menu) What item ID would you like to remove?: ";
		id = getDKP();

		if (id != -1)
		{
			std::cout << endl << "Removing [" << id << "]";

			printError = true;
			command = "SELECT ITEM FROM ITEM WHERE ID = " + to_string(id) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
			std::cout << "... " << endl;



			if (printError)
			{
				std::cout << "Error: No Entry For [" << id << "]" << endl << endl;
			}
			else
			{
				command = "DELETE FROM ITEM WHERE ID = " + to_string(id) + ";";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
				std::cout << endl;
				std::cout << "Removed [" << id << "] from the database! " << endl;

				printItems();
			}

		}
	}
}

void database::removePlayer()
{
	string command;
	string name = "~";

	while (!name.empty())
	{

		std::cout << "Remove Menu: [Enter] for Player Menu) Who would you like to remove?: ";
		name = getName();

		if (!name.empty())
		{
			std::cout << "Removing ";
			printError = true;

			command = "SELECT NAME FROM PLAYER WHERE NAME = '" + name + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			std::cout << "... " << endl;


			if (printError)
			{
				std::cout << "Error: No Entry For " << name << endl << endl;
			}
			else
			{
				command = "DELETE FROM PLAYER WHERE NAME = '" + name + "';";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
				std::cout << endl;
				std::cout << "Removed " << name << " from the database! " << endl;
				decPlayerCount();

				printPlayers();
			}
		}


		
	}

}

void database::removeRaid()
{
	string command;
	string dateFormat;
	int dateNum = 0;
	string date = "~";
	sqlite3_stmt* stmt = 0;

	while (!date.empty())
	{

		std::cout << "Remove Menu: [Enter] for Raid Menu) Enter Raid Date to remove(MMDDYYYY): ";
		dateNum = getDate(date);

		if (!date.empty())
		{
			std::cout << "Removing ";
			printError = true;

			command = "SELECT DATE FROM RAID WHERE DATE = " + to_string(dateNum) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackraid, 0, &zErrMsg);
			std::cout << "... " << endl;

			if (printError)
			{
				std::cout << "Error: Raid Date not found!" << endl;
			}
			else
			{
				command = "DELETE FROM RAID WHERE DATE = " + to_string(dateNum) + ";";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackraid, 0, &zErrMsg);

				revertDump(dateNum);

				printError = true;
				command = "SELECT * FROM FILECHECK WHERE FILE like '%RaidRoster-" + dateFormat + "%' LIMIT 1;";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				if (printError)
				{
					std::cout << "Error: No corresponding file found!" << endl;
				}
				else
				{
					command = "DELETE FROM FILECHECK WHERE FILE like '%RaidRoster-" + dateFormat + "%';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
				}

				std::cout << "Removed the " << date << " raid from the database!" << endl;

				command = "DELETE FROM RAIDDUMP WHERE DATE = " + to_string(dateNum) + ";";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
			}
		}


		
	}
}

void database::revertDump(int dateVal)
{
	string command;

	printError = true;
	command = "SELECT PLAYER,DKP FROM RAIDDUMP WHERE DATE = " + to_string(dateVal) + ";";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackremovedump, 0, &zErrMsg);

	if (printError)
	{
		std::cout << "Error: dateVal not found in raid dumps!" << endl;
	}
	else
	{
		for (int index = 0; index < globalIndex; index++)
		{
			command = globalCommand[index];
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			index++;

			command = globalCommand[index];
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
		}

		globalIndex = 0;
	}

}



void database::viewItem()
{
	string command;
	string name = "~";
	string input = "~";

	while (!name.empty())
	{
		name.clear();	

		std::cout << "View Menu: [Enter] for Item Menu) What item would you like to view?: ";
		name = getItem();


		if (isdigit(name[0]) && !name.empty())
		{
			printError = true;
			std::cout << "Loading ";
			command = "SELECT ITEM FROM ITEM WHERE ID = " + name + " LIMIT 1;";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

			int itemID;
			itemID = stoi(name);
			printItem(itemID);
		}
		else if (!name.empty())
		{
			printError = true;
			std::cout << "Loading ";
			command = "SELECT ITEM FROM ITEM WHERE ITEM = '" + name + "' LIMIT 1;";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

			printItem(name);

			while (!input.empty())
			{
				std::cout << "Item's Page: [Enter] for Item Menu) Viewing all " << name << ": View [B]uyer, [R]aid, [D]elete item, or [Q]uit";
				std::getline(std::cin,input);
				std::cout << endl;


				if (!input.empty())
				{
					input[0] = toupper(input[0]);

					if (input[0] == 'B')
					{
						viewPlayer();
					}
					else if (input[0] == 'R')
					{
						viewRaid();
					}
					else if (input[0] == 'D')
					{
						removeItem();
					}
					else
					{
						std::cout << "Error: no Such Option " << endl;
					}

					printItems();
				}

			}
		}
		else if (printError)
		{
			std::cout << "Error: No Entry For " << name << endl << endl;
		}


	}


	
}

void database::viewPlayer()
{
	string command;
	string name = "~";
	string dummy;
	string input = "~";

	while (!name.empty())
	{



		std::cout << "View Menu: [Enter] for Player Menu) Who would you like to view?: ";
		name = getName();
		

		if (!name.empty())
		{
			printError = true;
			std::cout << "Loading ";
			command = "SELECT NAME FROM PLAYER WHERE NAME = '" + name + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


			if (printError)
			{
				std::cout << "Error: No Entry For " << name << endl << endl;
			}
			else
			{
				while (!input.empty())
				{
					printPlayer(name);

					std::cout << "Player Page: [Enter] for View Menu) View all " << name << "'s [I]tems or [R]aids?: ";
					std::getline(std::cin,input);
					std::cout << endl;

					if (!input.empty())
					{
						input[0] = toupper(input[0]);

						if (input[0] == 'I')
						{
							printPlayerItems(name);

							std::cout << "Items Page: [Enter] for Player Page)...";
							std::getline(std::cin, dummy);
							std::cout << endl;
						}
						else if (input[0] == 'R')
						{
							printPlayerRaids(name);

							std::cout << "Raids Page: [Enter] for Player Page)...";
							std::getline(std::cin, dummy);
							std::cout << endl;
						}
						else
						{
							std::cout << "Error: no Such Option " << endl;
						}
					}


				}
				input = "~";
			}

			printPlayers();
		}

	}
}

void database::viewRaid()
{
	string input = "~";
	int dateNum = 0;
	bool leave = false;
	string command = "~";
	string dateForm = "~";

	while (!dateForm.empty())
	{
		std::cout << "Which raid would you like to view?(MMDDYYY): ";
		dateNum = getDate(dateForm);
		std::cout << endl;

		if (!dateForm.empty())
		{
			std::cout << "Loading";

			printError = true;
			command = "SELECT DATE FROM RAID WHERE DATE = " + to_string(dateNum) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
			std::cout << dateForm << "... " << endl;

			if (printError)
			{
				std::cout << "Error: Raid Date not found!" << endl;
			}
			else
			{
				printItems(dateNum);

				printRaid(dateNum);

				printDumpSummary(dateNum);
			}

		}
	}
	


}

void database::viewDumps()
{
	string input = "~";
	bool leave = false;
	string dateForm = "~";
	string player = "~";
	string event = "~";
	string command = "~";
	int dateNum = 0;


	while (!input.empty())
	{
		std::cout << "View Dumps Based on [D]ate, [E]vent or [P]layer: ";
		std::getline(std::cin, input);
		std::cout << endl;

		if (!input.empty())
		{
			input[0] = toupper(input[0]);

			if (input[0] == 'D')
			{
				std::cout << "What Raid Date Would You Like to View?(MMDDYYY): ";
				dateNum = getDate(dateForm);

				std::cout << "Loading ";

				printError = true;
				command = "SELECT DATE FROM RAIDDUMP WHERE DATE = " + to_string(dateNum) + ";";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
				std::cout << dateForm << "... " << endl;

				if (printError)
				{
					std::cout << "Error: Raid Date not found!" << endl;
				}
				else
				{
					printDumps(dateNum);
				}


			}
			else if (input[0] == 'E')
			{

				std::cout << "What Raid Event Would You Like to View?(MMDDYYY): ";
				event = getItem();

				std::cout << "Loading " << event << "..." << endl;

				printError = true;
				command = "SELECT * FROM RAIDDUMP WHERE EVENT = '" + event + "';";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				if (printError)
				{
					std::cout << "Error: No Such Event Found!" << endl;
				}
				else
				{
					command = "SELECT * FROM RAIDDUMP WHERE EVENT = '" + event + "';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
				}

			}
			else if (input[0] == 'P')
			{
				std::cout << "What Player's Dumps Would You Like to View?: ";
				player = getName();

				std::cout << "Loading " << player << "...";

				printError = true;
				command = "SELECT * FROM RAIDDUMP WHERE PLAYER = '" + player + "';";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				if (printError)
				{
					std::cout << "Error: No Such Player Found!" << endl;
				}
				else
				{
					command = "SELECT * FROM RAIDDUMP WHERE PLAYER = '" + player + "';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
				}

			}
			else
			{
				std::cout << "Error: No Such Option!" << endl;
			}


		}
	}

}



void database::updateItem()
{
	string command;

	int id = 0;


	while (id != -1)
	{

		std::cout << "Update Menu: [-1] for Main) What item id would you like to update?: ";
		id = getDKP();


		if (id != -1)
		{
			printError = true;

			printItem(id);
			std::cout << endl;


			if (printError)
			{
				std::cout << "Error: No Entry For " << id << endl << endl;
			}
			else
			{
				string input = "~";
				while (!input.empty()) //Item specific Edit Menu
				{

					std::cout << "[Enter] to leave: Update [I]tem name, [B]uyer, [D]KP spent or [R]aid date: ";
					std::getline(std::cin,input);
					std::cout << endl << endl;

					if (!input.empty())
					{
						input = toupper(input[0]);
						if (input == "I") //Changing Item Name
						{
							string name = "~";

							std::cout << "[Enter] to cancel: Enter New Item Name: ";
							name = getItem();

							if (!name.empty())
							{
								command = "UPDATE ITEM SET ITEM = '" + name + "' WHERE ID = " + to_string(id) + ";";
								sql = &command[0u];
								check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
							}

						}
						else if (input == "B") //Changing Item Buyer
						{

							string buyer = "~";
							while (!buyer.empty())
							{
								std::cout << "[Enter] to exit: Enter New Buyer: ";
								buyer = getName();

								if (!buyer.empty())
								{
									printError = true;
									command = "SELECT NAME FROM PLAYER WHERE NAME = '" + buyer + "';";
									sql = &command[0u];
									check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
									std::cout << endl;

									if (printError)
									{
										std::cout << "Error: Invalid buyer" << endl;
									}
									else
									{
										command = "UPDATE ITEM SET BUYER = '" + buyer + "' WHERE ID = " + to_string(id) + ";";
										sql = &command[0u];
										check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
										buyer = buyer.empty();
									}
								}

								


							}


						}
						else if (input == "D") //Changing DKP Spent
						{
							int dkp = 0;
							while (dkp != -1)
							{
								std::cout << "[-1] to exit) Enter New DKP Spent: ";
								dkp = getDKP();

								if (dkp != -1)
								{
									command = "UPDATE ITEM SET SPENT = " + to_string(dkp) + " WHERE ID = " + to_string(id) + ";";
									sql = &command[0u];
									check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
									dkp = -1;
									
								}

							}

						}
						else if (input == "R")
						{
							int date = 0;
							while (date != -1)
							{
								int dateVal;
								string testDate;
								std::cout << "[-1] to exit) Enter New Raid Date(MMDDYYYY): ";
								dateVal = getDate(testDate);

								if (dateVal != -1)
								{
									command = "UPDATE ITEM SET DATE = " + to_string(dateVal) + " WHERE ID = " + to_string(id) + ";";
									sql = &command[0u];
									check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);
									date = -1;
									
								}
							}

						}
						else
						{
							std::cout << "Error: No Such Option";
						}

						std::cout << endl;
						if (!input.empty())
						{
							printItem(id);
							std::cout << endl;
						}
						else if (input.empty())
						{
							printItems();
						}
					}

				}//end while - player specific

			}
		}

	}


}

void database::updatePlayer()
{
	string command;
	string name = "~";
	string newName;
	string newClass;

	while (!name.empty())
	{

		std::cout << "Update Menu: [Enter] for Main) Who would you like to update?: ";
		name = getName();
		std::cout << endl;


		if (!name.empty())
		{
			printError = true;

			printPlayer(name);
			/*command = "SELECT * FROM PLAYER WHERE NAME = '" + name + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);*/
			std::cout << endl;

			if (printError)
			{
				std::cout << "Error: No Entry For " << name << endl << endl;
			}
			else
			{
				string input = "~";
				while (!input.empty()) //Player specific Edit Menu
				{

					std::cout << "[Enter] to leave: Update [N]ame, [L]evel, [C]lass, [D]KP, [A]ttendance or an [I]tem: ";
					std::getline(std::cin,input);
					std::cout << endl << endl;

					if (!input.empty())
					{
						input = toupper(input[0]);

						if (input[0] == 'N')
						{
							std::cout << "[Enter] to cancel: Enter New Name: ";
							newName = getName();

							if (!newName.empty())
							{
								command = "UPDATE PLAYER SET NAME = '" + newName + "' WHERE NAME = '" + name + "';";
								sql = &command[0u];
								check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

								name = newName;
							}


						}
						else if (input[0] == 'L')
						{


							int level = 0;
							while (level != -1)
							{
								std::cout << "[-1] to exit: Enter New Level: ";
								level = getDKP();

								if (level != -1)
								{
									if (level < 1 || level > 110)
									{
										std::cout << "Error: Invalid level" << endl;
									}
									else
									{
										command = "UPDATE PLAYER SET Level = " + to_string(level) + " WHERE NAME = '" + name + "';";
										sql = &command[0u];
										check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
										level = -1;
									}
								}
							}

						}
						else if (input[0] == 'C')
						{
							std::cout << "[Enter] to cancel: Enter New Class: ";
							newClass = getName();

							if (!newClass.empty())
							{
								command = "UPDATE PLAYER SET Class = '" + newClass + "' WHERE NAME = '" + name + "';";
								sql = &command[0u];
								check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
							}

						}
						else if (input[0] == 'D')
						{
							int dkp = 0;
							while (dkp != -1)
							{
								std::cout << "[-1] to exit) Enter New DKP Amount: ";
								dkp = getDKP();

								if (dkp != -1)
								{
									command = "UPDATE PLAYER SET DKP = " + to_string(dkp) + " WHERE NAME = '" + name + "';";
									sql = &command[0u];
									check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
									dkp = -1;	
								}

							}


						}
						else if (input[0] == 'A')
						{
							float attendance = 0;
							while (attendance != -1)
							{
								std::cout << "[-1] to exit) Enter New 30 Day Attendance: ";
								std::cin >> attendance;
								std::cin.clear();
								std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

								if (attendance != -1)
								{
									if (attendance < 0 || attendance > 100)
									{
										std::cout << "Error: Invalid Percentage" << endl;
									}
									else
									{
										command = "UPDATE PLAYER SET Attendance = " + to_string(attendance) + " WHERE NAME = '" + name + "';";
										sql = &command[0u];
										check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
										attendance = -1;
									}
								}

							}

						}
						else if (input[0] == 'I')
						{
							updateItem();
						}
						else
						{
							std::cout << "Error: No Such Option" << endl;
						}

						if (!input.empty())
						{
							printPlayer(name);
							std::cout << endl;
						}
						else if (input.empty())
						{
							printPlayers();
						}

					}




				}//end while - player specific
			}

			
		}

	}
	
	
}



void database::parseDump(string directory,string fileName[],int fileNum)
{
	int fileCharCount = 26; //26 + 4 for .txt = 30 total for each file
	string command;

	//Struct to hold unknown players
	struct player
	{
		string newName;
		string newClass;
		int newLevel;
	};
	player unknown[100];


	//struct to gather data about each raid to later add to DB as a raid
	//parse item method should contain: items bought, dkp spent for raid
	struct raid
	{
		int totalDumps = 0;
		int dumpDKP = 0; //how much each dump is worth in dkp
		int dateInput = 0; // to use for adding, is the numerical date
		string date; // set in the dateform loop
		int singleDKP = 0; //set when asking for night's dkp val
		int raidDKP = 0; // set at the end, total raiders * individual dkp val
		int totalRaiders = 0; // incremented for each raider entered
		float avgRaiders = 0; //set at the end: total raiders / # of dumps = avg raiders
	};
	raid raidDetails[10];

	//Variable Declarations
			string charName;
		string charClass;
		string dummy;
		string flag;
		int charLevel;
		int groupNum;

		int raiderCount = 0;
		int raidIndex = 0;
		int fileCount = 0;
		int raidCount = 0;
		int fileIndex = 0;
		int dumpCount[10] = {0,0,0,0,0,0,0,0,0,0};
		int dumpTotal = 0;
		int dkpValue[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int dateInput[10] = { 0,0,0,0,0,0,0,0,0,0 };
		int dkpInput;
		string files[100];
	
		bool leave;
		string temp;
		string find;
		string input;
		string event;
		string eventStore;

		string dateForm[10];
		ifstream readFrom;



	//parse the names and dates, count the number of raids
	if (fileNum == 1)
	{
		temp = directory.substr(0, directory.length() - 1);

		printError = true;
		command = "SELECT * FROM FILECHECK WHERE FILE = '" + temp + "';";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

		if (printError)
		{
			command = "INSERT INTO FILECHECK(FILE) VALUES ('" + temp + "');";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			files[0] = temp;
			raidCount = 1;
			dumpCount[1] = 1;

			int place = files[0].length() - 20;

			dateForm[0] = files[0].substr(place + 5, 2) + '/';
			dateForm[0] += files[0].substr(place + 7, 2) + '/';
			dateForm[0] += files[0].substr(place + 1, 4);

			raidDetails[0].date = dateForm[0];


			dateInput[0] = ((temp[place + 1] - 48) * 10000000) + ((temp[place + 2] - 48) * 1000000) + ((temp[place + 3] - 48) * 100000)
				+ ((temp[place + 4] - 48) * 10000) + ((temp[place + 5] - 48) * 1000) + ((temp[place + 6] - 48) * 100)
				+ ((temp[place + 7] - 48) * 10) + (temp[place + 8] - 48);

			raidDetails[0].dateInput = dateInput[0];

		}
		else
		{
			std::cout << "Error: File Exists" << endl;
			fileNum = 0;
		}
		
	}
	else
	{

		for (int index = 0; index < fileNum; index++)
		{

			temp = directory + fileName[index];

			printError = true;
			command = "SELECT * FROM FILECHECK WHERE FILE = '" + temp + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			if (printError)
			{
				command = "INSERT INTO FILECHECK(FILE) VALUES ('" + temp + "');";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				files[fileIndex] = temp;
				int place = files[fileIndex].length() - 20;

				dateForm[fileIndex] = files[fileIndex].substr(place + 5, 2) + '/';
				dateForm[fileIndex] += files[fileIndex].substr(place + 7, 2) + '/';
				dateForm[fileIndex] += files[fileIndex].substr(place + 1, 4);

				//reorganizing the date to a standard format for input
				 





				if (dateForm[fileIndex] != dateForm[fileIndex - 1])
				{
					dateInput[0] = ((temp[place + 1] - 48) * 10000000) + ((temp[place + 2] - 48) * 1000000) + ((temp[place + 3] - 48) * 100000)
						+ ((temp[place + 4] - 48) * 10000) + ((temp[place + 5] - 48) * 1000) + ((temp[place + 6] - 48) * 100)
						+ ((temp[place + 7] - 48) * 10) + (temp[place + 8] - 48);

					raidDetails[raidIndex].dateInput = dateInput[fileIndex];

					raidDetails[raidIndex].date = dateForm[fileIndex];

					if (raidCount == 0)
					{
						raidCount = 1;
					}
					else
					{
						raidCount++;
					}

					dumpCount[raidCount]++;
					raidIndex++;

				}
				else
				{
					dumpCount[raidCount]++;
				}

				fileIndex++;
			}
			else
			{
				std::cout << temp << " Already Exists! Skipping...";
			}

		}
		fileNum = fileIndex;
	}


	//List the raid and dump totals

	std::cout << "Found " << raidCount << " raids!" << endl;
	for (int index = 1; index <= raidCount; index++)
	{
		std::cout << dumpCount[index] << " dumps for raid #" << index << endl;
		dumpTotal += dumpCount[index];
	}

	
	//assign DKP to dumps
	int PH = 0;
	for (int index = 1; index <= raidCount; index++)
	{
		std::cout << "[0] to make no change) There were " << dumpCount[index] << " dumps for raid #" << index << ", how much dkp was each dump worth? : ";
		dkpInput = getDKP();
		std::cout << endl;

		if (dkpInput != 0)
		{
			std::cout << "Adding " << dkpInput << " to each present player, for a night's total of " << dkpInput * dumpCount[index] << " DKP!" << endl;
			raidDetails[index - 1].singleDKP = dkpInput * dumpCount[index];
			raidDetails[index - 1].dumpDKP = dkpInput;
		}

		int dkpIndex = 0;
		while (dkpIndex < dumpCount[index])
		{
			dkpValue[PH] = dkpInput;
			dkpIndex++;
			PH++;
		}

	}




	raidIndex = 0;
	//We have files to parse, now parse each one into the DB
	for (int index = 0; index < fileNum; index++)
	{
		readFrom.open(files[index]);
		//reset variables for each new file
				string timeForm;
		int unknownCount = 0;
		int dumpPlayers = 0;
		leave = false;


        //find and format the time of dump
		int place = files[index].length() - 10;
		string temp = files[index];

		int timeTest = ((temp[place] - 48) * 10) + (temp[place + 1] - 48);
		int time = ((temp[place] - 48) * 1000) + ((temp[place + 1] - 48) * 100);
		int time2 = ((temp[place + 2] - 48) * 10) + (temp[place + 3] - 48);


		//convert to standard
		if (timeTest > 12 )
		{
			timeTest -= 12;
		}

		//format time
		if (time2 < 10)
		{
			timeForm = to_string(timeTest) + ":0" + to_string(time2);
		}
		else
		{
			timeForm = to_string(timeTest) + ":" + to_string(time2);
		}
		time += time2;



		//Check for a change in raid date
		if ((dateForm[index] != dateForm[index - 1]) && index > 0)
		{
			std::cout << "New Raid Day! Dump is now for: " << dateForm[index] << endl;
			raidIndex++;
		}

		//Get Input for the Event on the Dump
		while (!leave)
		{
			//get the event name per dump
			std::cout << "Raid #" << raidIndex + 1 << " : Dump #" << index + 1 << " - " << endl;
			std::cout << "What event is the dump on " << dateForm[index] << " at " << timeForm << " for? [Enter] to repeat last event: ";
			event = getItem();
			std::cout << endl;

			if (event.empty() && eventStore.empty())
			{
				std::cout << "Error: No prior event listed!" << endl;
			}
			else if (event.empty())
			{
				event = eventStore;
				leave = true;
			}
			else
			{
				eventStore = event;
				leave = true;
			}

		}

		//Output date/time info for error checking
				std::cout << "--------------------------------------" << endl;
		std::cout << "| Alpha  : Date = " << dateForm[index] <<  " Time = " << timeForm << " |" << endl;
		std::cout << "| Numeric: Date = " << dateInput[index] << " Time = " << time << " |" << endl;
		std::cout << "--------------------------------------" << endl;
		std::cout << "| Player Name               Exists?  |" << endl;
		std::cout << "--------------------------------------" << endl;

		//grab all player data from the file, add unknowns to array
		while (!readFrom.eof())
		{

			readFrom >> groupNum;
			readFrom >> charName;
			readFrom >> charLevel;
			readFrom >> charClass;	
			std::getline(readFrom, flag);

			if (!readFrom.eof())
			{
				//add dumps to player
				printError = true;
				command = "SELECT NAME FROM PLAYER WHERE NAME = '" + charName + "';";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
				temp = formColumn(charName,20);
				std::cout << "| " << temp;

				if (printError) //returns true if the player is not in the system already
				{
					std::cout << "No" << endl;
					unknown[unknownCount].newName = charName;
					unknown[unknownCount].newClass = charClass;
					unknown[unknownCount].newLevel = charLevel;
					unknownCount++;
				}
				else
				{
					std::cout << "Yes" << endl;
					command =  "INSERT INTO RAIDDUMP(DATE,TIME,EVENT,PLAYER,DKP,FILE) VALUES";
					command += " (" + to_string(dateInput[raidIndex]) + ", " + to_string(time) + ", '" + event + "', '" + charName + "', " + to_string(dkpValue[index]) + ", '" + files[index] + "');";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

					command = "UPDATE PLAYER SET CHECKS = CHECKS + 1, DKP = DKP + " + to_string(dkpValue[index]) + ", DKPTOTAL = DKPTOTAL + " + to_string(dkpValue[index]) + " WHERE NAME = '" + charName + "';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

				}
				dumpPlayers++;
				raidDetails[raidIndex].totalRaiders++;
			}
			


		}

		std::cout << "------------------------------------" << endl;
		std::cout << "Raiders found in this dump: " << dumpPlayers << endl;


		//Go through the unknown players, and add any to the DB
		if (unknownCount > 0) 
		{
			//Can skip adding entirely
			std::cout << "Found " << unknownCount << " unknown Characters, would you like to add any? (Y/N): ";
			std::getline(std::cin,input);
			std::cout << endl;

			if (toupper(input[0]) == 'Y')
			{

				int index2 = 0;

				while (!input.empty() && index2 < unknownCount)
				{
					std::cout << "[Enter] to Leave: Add " << unknown[index2].newName << " into the database? (Y/N): ";
					std::getline(std::cin, input);
					std::cout << endl;


					if (toupper(input[0]) == 'Y')
					{


						if (addPlayer(unknown[index2].newName, unknown[index2].newClass, unknown[index2].newLevel)) //addPlayer returns bool: 1 on success
						{

							command =  "INSERT INTO RAIDDUMP(DATE,TIME,EVENT,PLAYER,DKP,FILE) VALUES";
							command += " (" + to_string(dateInput[raidIndex]) + ", " + to_string(time) + ", '" + event + "', '" + unknown[index2].newName + "', " + to_string(dkpValue[index]) + ", '" + files[index] + ");";
							sql = &command[0u];
							check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

							command = "UPDATE PLAYER SET CHECKS = CHECKS + 1, DKP = DKP + " + to_string(dkpValue[index]) + ", DKPTOTAL = DKPTOTAL + " + to_string(dkpValue[index]) + " WHERE NAME = '" + unknown[index2].newName + "';";
							sql = &command[0u];
							check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

							std::cout << unknown[index2].newName << " added to the dump!" << endl;

						}
						else
						{
							std::cout << "Add Failed" << endl;
						}
					}

					if (toupper(input[0]) == 'N')
					{
						std::cout << "Looking at next unknown player... " << endl;
					}
						
					index2++;

				}
				
			}
			else
			{
				std::cout << "Skipping adds..." << endl;
			}
			

		}


		std::cout << "Done with this dump!" << endl;
		raidDetails[raidIndex].totalDumps++;
		readFrom.close();
	}

	//Raid Summary and Adds/Updates the Raid Info
	for (int index = 0; index < raidCount; index++)
	{

		raidDetails[index].avgRaiders = raidDetails[index].totalRaiders / float(dumpCount[index + 1]);
		raidDetails[index].raidDKP = raidDetails[index].totalRaiders * raidDetails[index].dumpDKP;

		std::cout << "--------raidDetails Report---------" << endl;
		std::cout << "Raid Date   : " << raidDetails[index].date << endl;
		std::cout << "Date Num    : " << raidDetails[index].dateInput << endl << endl;

		std::cout << "Raid DKP    : " << raidDetails[index].raidDKP << endl;
		std::cout << "Single DKP  : " << raidDetails[index].singleDKP << endl;
		std::cout << "Dump DKP    : " << raidDetails[index].dumpDKP << endl << endl;

		std::cout << "Raider Total: " << raidDetails[index].totalRaiders << endl;
		std::cout << "Raider AVG  : " << raidDetails[index].avgRaiders << endl;
		std::cout << "Total Dumps : " << raidDetails[index].totalDumps << endl;
		std::cout << "-----------------------------------" << endl;

		printError = true;
		command = "SELECT * FROM RAID WHERE date = " + to_string(raidDetails[index].dateInput) + ";";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

		if(printError)
		{
			//"INSERT INTO RAID(DATE,ITEMS,DKPEARNED,RAIDERS,RAIDERSAVG, DKPSPENT, DKPRAID) VALUES (12202017, 47, 158, 52, 48.5, 7425, 8216);";
			command = "INSERT INTO RAID(DATE,ITEMS,DKPEARNED,RAIDERS,RAIDERSAVG,DKPSPENT,DKPRAID,DUMPS) VALUES (" \
									//DATE                      //ITEMS                 //DKPEARNED
				    + to_string(raidDetails[index].dateInput) + ", 0 ," + to_string(raidDetails[index].singleDKP) + ", " 
									//RAIDERS                                           //RAIDERSAVG                //DKPSPENT
				    + to_string(raidDetails[index].totalRaiders) + ", " + to_string(raidDetails[index].avgRaiders) + ", 0 ," 
									//DKPRAID
				    + to_string(raidDetails[index].raidDKP) + ", " + to_string(raidDetails[index].totalDumps) + ");";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			std::cout << "Added a new Raid!" << endl;
		}
		else
		{
			command = "UPDATE RAID SET"					//DKP EARNED 2
				" DKPEARNED = DKPEARNED + " + to_string(raidDetails[index].singleDKP) +
													    //TOTAL RAIDERS 2
				",RAIDERS = RAIDERS + " + to_string(raidDetails[index].totalRaiders) +

				",DUMPS = DUMPS + " + to_string(raidDetails[index].totalDumps) +
														//DKPRAID 2
				",DKPRAID = DKPRAID + " + to_string(raidDetails[index].raidDKP) + 
				
														//SHARED DATE
				" WHERE DATE = " + to_string(raidDetails[index].dateInput) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			command = "UPDATE RAID SET" 	
				" RAIDERSAVG = (CAST(RAIDERS AS FLOAT) / DUMPS) "
				" WHERE DATE = " + to_string(raidDetails[index].dateInput) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			std::cout << "Modified an existing raid!" << endl;
		}
	}

	//Update the total checks and all existing player attendance
		command = "UPDATE PLAYER SET CHECKSTOTAL = CHECKSTOTAL + " + to_string(dumpTotal) + ";";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

	command = "UPDATE PLAYER SET ATTENDANCE = (CAST(CHECKS AS FLOAT)/CHECKSTOTAL) * 100;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

}



// ugh - the prepared statements with select. wtf man, dont waste too much time here.
// fixed up how dates are stored, all storage is done in YYYYMMDD to be ablt to properly order them
//but all input/output/display is done MMDDYYYY since thats the normal way to view it. Check there arent 
//rogue areas where thats swapped.

//Besides the selects/prepared shitshow, its looking solid. maybe able to make a function for a few more things like forming a date
//or other things that get repeated.
void database::parseLoots(string directory, string fileName[], int fileNum)
{
	//variable declarations
	struct itemSummary
	{
		string date;
		int dateInput = 0;
		int items = 0;
		int dkpSpent = 0;
	};
	itemSummary raidDetails[10];

	ifstream readFrom;
	string temp;
	string command;
	string lootLine;
	string itemName;
	string buyerName;
	string input;
	int dkpVal = 0;
	string dateForm[10];
	int dateInput[10] = {0,0,0,0,0,0,0,0,0,0};
	int dumpCount[10] = {0,0,0,0,0,0,0,0,0,0};
	int fileIndex = 0;
	int val[5] = {0,0,0,0,0};
	int valMult[5] = {10000,1000,100,10,1};
	int raidIndex = 0;
	int raidCount = 0;
	int check = 0;
	bool leave = false;
	bool found = false;
	bool grats = false;
	char find = '~';
	string files[10];

	int columnWidth[10] = {34,15,15};


	//parse the names and dates, count the number of raids
	if (fileNum == 1)
	{
		temp = directory.substr(0, directory.length() - 1);

		printError = true;
		command = "SELECT * FROM FILECHECK WHERE FILE = '" + temp + "';";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

		if (printError)
		{
			command = "INSERT INTO FILECHECK(FILE) VALUES ('" + temp + "');";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			std::cout << "files[0]: " << temp << endl;
			files[0] = temp;
			raidCount = 1;
			dumpCount[1] = 1;

			int place = files[0].length() - 13;

			dateForm[0] = files[0].substr(place + 5, 2) + '/';
			dateForm[0] += files[0].substr(place + 7, 2) + '/';
			dateForm[0] += files[0].substr(place + 1, 4);

			raidDetails[0].date = dateForm[0];


			dateInput[0] = ((temp[place + 1] - 48) * 10000000) + ((temp[place + 2] - 48) * 1000000) + ((temp[place + 3] - 48) * 100000)
				+ ((temp[place + 4] - 48) * 10000) + ((temp[place + 5] - 48) * 1000) + ((temp[place + 6] - 48) * 100)
				+ ((temp[place + 7] - 48) * 10) + (temp[place + 8] - 48);

			raidDetails[0].dateInput = dateInput[0];

		}
		else
		{
			std::cout << "Error: File Exists" << endl;
			fileNum = 0;
		}

	}
	else
	{

		for (int index = 0; index < fileNum; index++)
		{

			temp = directory + fileName[index];

			printError = true;
			command = "SELECT * FROM FILECHECK WHERE FILE = '" + temp + "';";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			if (printError)
			{
				command = "INSERT INTO FILECHECK(FILE) VALUES ('" + temp + "');";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				files[fileIndex] = temp;
				int place = files[fileIndex].length() - 13;

				dateForm[fileIndex] = files[fileIndex].substr(place + 5, 2) + '/';
				dateForm[fileIndex] += files[fileIndex].substr(place + 7, 2) + '/';
				dateForm[fileIndex] += files[fileIndex].substr(place + 1, 4);

				//reorganizing the date to a standard format for input






				if (dateForm[fileIndex] != dateForm[fileIndex - 1])
				{
					dateInput[fileIndex] = ((temp[place + 1] - 48) * 10000000) + ((temp[place + 2] - 48) * 1000000) + ((temp[place + 3] - 48) * 100000)
						+ ((temp[place + 4] - 48) * 10000) + ((temp[place + 5] - 48) * 1000) + ((temp[place + 6] - 48) * 100)
						+ ((temp[place + 7] - 48) * 10) + (temp[place + 8] - 48);

					raidDetails[raidIndex].dateInput = dateInput[fileIndex];

					raidDetails[raidIndex].date = dateForm[fileIndex];

					if (raidCount == 0)
					{
						raidCount = 1;
					}
					else
					{
						raidCount++;
					}

					dumpCount[raidCount]++;
					raidIndex++;

				}
				else
				{
					dumpCount[raidCount]++;
				}

				fileIndex++;
			}
			else
			{
				std::cout << temp << " Already Exists! Skipping...";
			}

		}
		fileNum = fileIndex;
	}


	//List the raid and dump totals
	std::cout << "Found " << raidCount << " raid nights!" << endl;

	raidIndex = 0;
	//We have files to parse, now parse each one into the DB
	for (int index = 0; index < fileNum; index++)
	{
		readFrom.open(files[index]);
		readFrom >> noskipws;
		//reset variables for each new file
		string timeForm;
		int unknownCount = 0;
		int dumpPlayers = 0;
		int index2 = 0;
		leave = false;
		found = false;


		//Check for a change in raid date
		if ((dateForm[index] != dateForm[index - 1]) && index > 0)
		{
			std::cout << "New Raid Day! Loots are now for: " << dateForm[index] << endl;
			raidIndex++;
		}

		//grab all item data from the line
		while (!readFrom.eof())
		{
			//Variable resets
			purchase buy;
			string getlineTest;
			lootLine = "";
			buyerName = "";
			itemName = "";
			temp = "";
			dkpVal = 0;
			index2 = 0;
			leave = false;
			bool modify = true;
			grats = false;
			

			readFrom >> find;

			std::getline(readFrom,lootLine,'\n');

			buy = parseLootLine(lootLine,db);

			//Handle an error if there is one
			if (!buy.complete)
			{
				std::cout << "Original Text Line - " << endl << lootLine << endl;
				std::cout << "Could not find a " << buy.errorFlag << " value!" << endl;
				
				
				while (modify)
				{
					std::cout << "Current Purchase Details :" << endl;
					std::cout << "Item Name  : " << buy.itemName  << endl;
					std::cout << "Buyer Name : " << buy.buyerName << endl;
					std::cout << "DKP Value  : " << buy.dkpVal    << endl;

					std::cout << "[Enter] to finalize transaction - Modify [I]tem, [B]uyer, [D]KP?";
					std::getline(std::cin, input);
					std::cout << endl;

					if (input.empty())
					{
						modify = false;
					}
					else if (toupper(input[0]) == 'I')
					{
						std::cout << "Enter the item name: ";
						buy.itemName = getItem();
					}
					else if (toupper(input[0]) == 'B')
					{
						std::cout << "Enter the player's name: ";
						buy.buyerName = getName();
						std::cout << endl;
					}
					else if (toupper(input[0]) == 'D')
					{
						std::cout << "Enter the dkp value: ";
						buy.dkpVal = getDKP();
					}
					else
					{
						std::cout << "No such option!" << endl;
					}
				}
				std::cout << endl;

			}

			if (!readFrom.eof())
			{

				//Set globalString to player's type
				printError = true;
				buy.buyerName[0] = toupper(buy.buyerName[0]);
				command = "SELECT TYPE FROM PLAYER WHERE NAME = '" + buy.buyerName + "';";
				sql = &command[0u];
				check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

				if (printError)
				{
					std::cout << "Error: Player not found! No entry for: " << buy.buyerName << endl <<
					             "Would you like to [A]dd them, [C]hange the buyer or [S]kip this entry?";
					std::getline(std::cin, input);
					std::cout << endl;

					//Menu options
					input[0] = toupper(input[0]);
					if (input[0] == 'A')
					{
						addPlayer(buy.buyerName);
					}
					else if (input[0] == 'S')
					{
						leave = true;
					}
					else if (input[0] == 'C')
					{
						std::cout << "Who is the buyer?: ";
						buy.buyerName = getName();
					}
					else
					{
						std::cout << "Error: No such Option!" << endl;
					}
					//Set globalString to player's type
					buy.buyerName[0] = toupper(buy.buyerName[0]);
					command = "SELECT TYPE FROM PLAYER WHERE NAME = '" + buy.buyerName + "';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);
				}


				string spaces = "";
				int dkpTest = buy.dkpVal;
				int dkpDigits = 0;
				int secondDKP = buy.dkpVal;

				//print a check and add items to DB
				std::cout << buy.logLine << endl;
				std::cout << "       Item                         DKP            Name           Type     " << endl;
				std::cout << "---------------------------------------------------------------------------" << endl;



				temp = formColumn(buy.itemName, columnWidth[0]);
				std::cout << "  " << temp;

				temp = formColumn(buy.dkpVal, columnWidth[1]);
				std::cout << temp;

				temp = formColumn(buy.buyerName,columnWidth[2]);
				std::cout << temp;
				

				std::cout << globalString;
				std::cout << endl;
				dkpTest = secondDKP;
				dkpDigits = 0;

				if (buy.errorFlag == "x2")
				{
					temp = formColumn(buy.itemName,columnWidth[0]);
					std::cout << "  " << temp;

					temp = formColumn(secondDKP,columnWidth[1]);
					std::cout << temp;

					temp = formColumn(buy.buyerName, columnWidth[2]);
					std::cout << temp;
					std::cout << globalString;
					std::cout << endl << endl;
				}
				else if (buy.errorFlag == "two winners")
				{
					//Finding secondBuyer's Type
					buy.secondBuyer[0] = toupper(buy.secondBuyer[0]);
					command = "SELECT TYPE FROM PLAYER WHERE NAME = '" + buy.secondBuyer + "';";
					sql = &command[0u];
					check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

					temp = formColumn(buy.itemName,columnWidth[0]);
					std::cout << "  " << temp;

					temp = formColumn(secondDKP,columnWidth[1]);
					std::cout << temp;
					
					temp = formColumn(buy.secondBuyer,columnWidth[2]);
					std::cout << temp;
					std::cout << globalString;
					std::cout << endl << endl;
				}
				else
				{
					std::cout << endl;
				}



				//Confirm the item info to be added with user
				while (!leave)
				{
					std::cout << "[Enter] the item with this information? Undo [B]ox Triple/[S]kip/[M]odify :";
					std::getline(std::cin, input);
					std::cout << endl;


					if (input.empty())
					{
						if (buy.errorFlag == "x2")
						{
							std::cout << "Two Items Added for " << buy.buyerName << "!" << endl;
							addItem(raidDetails[raidIndex].dateInput, buy.buyerName, buy.itemName, buy.dkpVal);
							addItem(raidDetails[raidIndex].dateInput, buy.buyerName, buy.itemName, buy.dkpVal);
						}
						else if (buy.errorFlag == "two winners")
						{
							std::cout << "Added Items for " << buy.buyerName << " and " << buy.secondBuyer << "!" << endl;
							addItem(raidDetails[raidIndex].dateInput, buy.buyerName, buy.itemName, buy.dkpVal);
							addItem(raidDetails[raidIndex].dateInput, buy.secondBuyer, buy.itemName, secondDKP);
						}
						else
						{
							addItem(raidDetails[raidIndex].dateInput, buy.buyerName, buy.itemName, buy.dkpVal);
						}
						
						std::cout << "Added!" << endl;
						leave = true;

						raidDetails[raidIndex].dkpSpent += buy.dkpVal;
						raidDetails[raidIndex].items++;
					}
					else if (toupper(input[0]) == 'B')
					{
						buy.dkpVal = buy.dkpVal / 3;
						std::cout << "DKP Triple Undone: " << buy.dkpVal / 3;
						std::cout << " to (" << buy.dkpVal << ") for box! :" << endl;
						addItem(raidDetails[raidIndex].dateInput, buy.buyerName, buy.itemName, buy.dkpVal);
						std::cout << "Added!" << endl;
						leave = true;

						raidDetails[raidIndex].dkpSpent += buy.dkpVal;
						raidDetails[raidIndex].items++;
					}
					else if (toupper(input[0]) == 'M')
					{
						while (!input.empty())
						{
							std::cout << "[Enter] to leave: Change the [I]tem, [B]uyer or [D]kp?:";
							std::getline(std::cin, input);
							std::cout << endl;

							if (toupper(input[0]) == 'I')
							{
								std::cout << "Enter a new Item Name: ";
								buy.itemName = getItem();
							}
							else if (toupper(input[0]) == 'B')
							{
								std::cout << "Enter a new Buyer Name: ";
								buy.buyerName = getName();

							}
							else if (toupper(input[0]) == 'D')
							{
								std::cout << "Enter a DKP amount: ";
								buy.dkpVal = getDKP();
							}
							else
							{
								std::cout << "Error: No such Option!" << endl;
							}

							spaces = "";
							int dkpTest = buy.dkpVal;
							int dkpDigits = 0;

							command = "SELECT TYPE FROM PLAYER WHERE NAME = '" + buy.buyerName + "';";
							sql = &command[0u];
							check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

							std::cout << "       Item                         DKP            Name           Type     " << endl;
							std::cout << "---------------------------------------------------------------------------" << endl;

							//check/orint the item name

							temp = formColumn(buy.itemName, columnWidth[0]);
							std::cout << "  " << temp;

							temp = formColumn(buy.dkpVal, columnWidth[1]);
							std::cout << temp;

							temp = formColumn(buy.buyerName, columnWidth[2]);
							std::cout << temp;


							std::cout << globalString;
							std::cout << endl << endl;
						}
						raidDetails[raidIndex].dkpSpent += buy.dkpVal;
						raidDetails[raidIndex].items++;

					}
					else if (toupper(input[0]) == 'S') //returns true if the player is not in the system already
					{
						std::cout << "Skipping..." << endl;
						leave = true;
					}
					else
					{
						std::cout << "Error: No such Option!" << endl;
					}

				}
				
			}

		}
		//Raid Summary
		std::cout << "---------------------------------------" << endl;
		std::cout << "Items found in this raid     : " << raidDetails[raidIndex].items << endl;
		std::cout << "Total dkp spent in this raid : " << raidDetails[raidIndex].dkpSpent << endl;
		std::cout << "---------------------------------------" << endl;


		std::cout << "Done with this dump!" << endl;
		readFrom.close();
	}

	//Raid Summary and Adds/Updates the Raid Info
	for (int index = 0; index < raidCount; index++)
	{
		//title heading
		std::cout << "--------raidDetails Report---------" << endl;
		std::cout << "Raid Date   : " << raidDetails[index].date << endl;
		std::cout << "Date Num    : " << raidDetails[index].dateInput << endl << endl;

		std::cout << "Item Total  : " << raidDetails[index].items << endl;
		std::cout << "Raid DKP    : " << raidDetails[index].dkpSpent << endl;
		std::cout << "-----------------------------------" << endl;

		//See if Raid Exists
		printError = true;
		command = "SELECT * FROM RAID WHERE date = " + to_string(raidDetails[index].dateInput) + ";";
		sql = &command[0u];
		check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

		//Check for existing raid to modify, or add a new raid
		if (printError)
		{
			//Make new Raid
			//"INSERT INTO RAID(DATE,ITEMS,DKPEARNED,RAIDERS,RAIDERSAVG, DKPSPENT, DKPRAID) VALUES (12202017, 47, 158, 52, 48.5, 7425, 8216);";
			command = "INSERT INTO RAID(DATE,ITEMS,DKPEARNED,RAIDERS,RAIDERSAVG,DKPSPENT,DKPRAID) VALUES (" \
				//DATE                                                //ITEMS                         //DKPEARNED
				+ to_string(raidDetails[index].dateInput) + ", " + to_string(raidDetails[index].items) + " ,0, "
			//RAIDERS //RAIDERSAVG       //DKPSPENT                          //DKPRAID
				"0,     0, "        + to_string(raidDetails[index].dkpSpent) + " ,0);";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			std::cout << "Added a new Raid!" << endl;
		}
		else
		{
			//Update existing raid
			command = "UPDATE RAID SET "				
				      "ITEMS = ITEMS + " + to_string(raidDetails[index].items) +
				      ",DKPSPENT = DKPSPENT + " + to_string(raidDetails[index].dkpSpent) +
			       	  " WHERE DATE = " + to_string(raidDetails[index].dateInput) + ";";
			sql = &command[0u];
			check = sqlite3_exec(db, sql, callbackno, 0, &zErrMsg);

			std::cout << "Modified an existing raid!" << endl;
		}
	}

}


void database::printPlayers()
{

	std::cout << "||***********************************************************************************************||" << endl;
	std::cout << "|| Name          Level      Class          Type    DKP      Earned    30 Day   Life     Checks   ||" << endl;
	std::cout << "||-----------------------------------------------------------------------------------------------||" << endl; //added 20

	sql = "SELECT * FROM PLAYER ORDER BY ATTENDANCE DESC;";
	check = sqlite3_exec(db,sql,callback,0, &zErrMsg);


	std::cout << "||***********************************************************************************************||" << endl;

}

void database::printPlayer(string name)
{
	string command;
	string asterisks(37 - name.length(), '*');

	std::cout << "||******************************* Viewing " << name << "'s page!" << asterisks << "||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "|| Name           Class          Type    DKP      Earned    30 Day   Life     Checks   ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl; //added 20

	command = "SELECT NAME,CLASS,TYPE,DKP,DKPTOTAL,ATTENDANCE,LIFETIME,CHECKS,CHECKSTOTAL FROM PLAYER WHERE NAME = '" + name + "';";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackplayerinfo, 0, &zErrMsg);


	string asterisksAgain(38 - name.length(), '*');
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||********************************* " << name << "'s Purchases " << asterisksAgain << "||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date          Buyer          Item Name                               Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM WHERE BUYER = '" + name + "';"; 
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;
}

void database::printPlayerItems(string name)
{
	string command;

	string asterisksAgain(38 - name.length(), '*');
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||********************************* " << name << "'s Purchases " << asterisksAgain << "||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date          Buyer          Item Name                               Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM WHERE BUYER = '" + name + "';";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;

}

void database::printPlayerRaids(string name)
{
	string command;
	string asterisksAgain(35 - name.length(), '*');

	std::cout << "||************************* " << name << "'s Raids " << asterisksAgain << "||" << endl;
	std::cout << "||                                                                      ||" << endl;
	std::cout << "|| Raid Day/Date | # Items Bought | DKP Spent | DKP Earned | Raid Total ||" << endl;
	std::cout << "||----------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAID WHERE BUYER = '" + name + "';";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackraid, 0, &zErrMsg);
	std::cout << "||                                                                      ||" << endl;
	std::cout << "||**********************************************************************||" << endl;
}




void database::printItems()
{
	string command;

	std::cout << "||************************************ Item Database **********************************||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date        Buyer        Item Name                                   Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;

}

void database::printItems(int date)
{
	string command;

	std::cout << "||************************************ Item Database **********************************||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date        Buyer        Item Name                                   Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM WHERE DATE = " + to_string(date) + ";";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;

}

void database::printItem(string name)
{
	string command;

	string asterisksAgain(40 - name.length(), '*');
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||********************************* " << name << "'s History " << asterisksAgain << "||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date        Buyer        Item Name                                   Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM WHERE ITEM = '" + name + "';";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;

}

void database::printItem(int id)
{
	string command;

	string asterisksAgain(45 - to_string(id).length(), '*'); //Slow running, but simple way to do this (Could make a loop to determine)
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||********************************* Item #" << id << asterisksAgain << "||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||  ID     Date        Buyer        Item Name                                   Spent  ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM ITEM WHERE ID = " + to_string(id) + ";";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackitem, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;

}

void database::printRaids()
{
	string command;

	//int column[10] = { 15, 9, 13, 10, 12, 11};


	//sql = "CREATE TABLE RAID(DATE INT UNIQUE,ITEMS INT,DKPEARNED INT,RAIDERS INT,RAIDERSAVG FLOAT, DKPSPENT INT, DKPRAID INT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	std::cout << "||************************************** Raid List ************************************||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "|| Raid Date    | Items | DKP Earned | Raiders | Avg. Raiders | DKP Spent | Raid DKP   ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAID;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackraid, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;
}

void database::printRaid(int date)
{
	string command;

	//int column[10] = { 15, 9, 13, 10, 12, 11};


	//sql = "CREATE TABLE RAID(DATE INT UNIQUE,ITEMS INT,DKPEARNED INT,RAIDERS INT,RAIDERSAVG FLOAT, DKPSPENT INT, DKPRAID INT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	std::cout << "||************************************** Raid List ************************************||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "|| Raid Date    | Items | DKP Earned | Raiders | Avg. Raiders | DKP Spent | Raid DKP   ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAID WHERE DATE = " + to_string(date) + ";";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackraid, 0, &zErrMsg);

	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;
}


void database::printDumps()
{
	string command;

	//		int column[10] = { 15, 15, 39, 15};
	//sql = "CREATE TABLE RAIDDUMP(DATE INT,TIME INT,EVENT TEXT,PLAYER TEXT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	std::cout << "||*************************************** Raid Dumps ***************************************||" << endl;
	std::cout << "||                                                                                          ||" << endl;
	std::cout << "|| Raid Date    | Dump Time    | Event                                | Raider        | DKP ||" << endl;
	std::cout << "||------------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAIDDUMP ORDER BY DATE DESC;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

	std::cout << "||                                                                                          ||" << endl;
	std::cout << "||******************************************************************************************||" << endl;
}

void database::printDumps(int date)
{
	string command;

	//		int column[10] = { 15, 15, 39, 15};
	//sql = "CREATE TABLE RAIDDUMP(DATE INT,TIME INT,EVENT TEXT,PLAYER TEXT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	std::cout << "||*************************************** Raid Dumps ***************************************||" << endl;
	std::cout << "||                                                                                          ||" << endl;
	std::cout << "|| Raid Date    | Dump Time    | Event                                | Raider        | DKP ||" << endl;
	std::cout << "||------------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAIDDUMP WHERE DATE = " + to_string(date) + " ORDER BY DATE DESC;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);

	std::cout << "||                                                                                          ||" << endl;
	std::cout << "||******************************************************************************************||" << endl;
}
//Working one fleshing out the system. Stopped while adding a callback - dumpsummary. it currently prints out all dumps,
//need to make it print out only one dump for each unique time. Plan on using a global int/string to keep track of
//how many players were there for the event thats being printed, use the string to keep track of the event being looked at
//just thought - maybe use time instead so that duplicate events dont screw with it
void database::printDumpSummary(int date)
{
	string command;

	//		int column[10] = { 15, 15, 39, 15};
	//sql = "CREATE TABLE RAIDDUMP(DATE INT,TIME INT,EVENT TEXT,PLAYER TEXT);";
	//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


	std::cout << "||*************************************** Raid Dumps **********************************||" << endl;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "|| Raid Date    | Dump Time    | Event                                | DKP  | Raiders ||" << endl;
	std::cout << "||-------------------------------------------------------------------------------------||" << endl;

	command = "SELECT * FROM RAIDDUMP WHERE DATE = " + to_string(date) + " ORDER BY TIME DESC;";
	sql = &command[0u];
	check = sqlite3_exec(db, sql, callbackdumpsummary, 0, &zErrMsg);
	std::cout << globalInt << "      ||" << endl;
	globalInt = 0;
	std::cout << "||                                                                                     ||" << endl;
	std::cout << "||*************************************************************************************||" << endl;
}


void database::incPlayerCount()
{
	std::cout << "Players: " << playerCount << " -> ";
	playerCount++;
	std::cout << playerCount << endl;
}

void database::decPlayerCount()
{
	playerCount--;
}

void database::incItemCount()
{
	itemCount++;
}



database::database()
{
	ifstream ifile(stats);
	string dummy;
	int players;
	int raids;
	int items;

	//stats format--
	//items x
	//players y
	//raids z

	if (sqlite3_open("DKP Site", &db))
	{
		std::cout << "Failed to open Database" << endl;
	}
	else
	{
		std::cout << "Database opened!" << endl;
	}


	if (ifile.is_open())
	{
		ifile >> dummy;
		ifile >> items;
		ifile >> dummy;
		ifile >> players;
		ifile >> dummy;
		ifile >> raids;

		playerCount = players;
		itemCount = items;
		raidCount = raids;

	}
	else
	{
		std::cout << "Error: Stats File Not Found (Input)" << endl;
	}

}

database::~database()
{
	ofstream ofile(stats);
	string dummy;


	if (ofile.is_open())
	{
		ofile << "items " << itemCount << endl;
		ofile << "players " << playerCount << endl;
		ofile << "raids " << raidCount << endl;

	}
	else
	{
		std::cout << "Error: Stats File Not Found (Output)" << endl;
	}

	sqlite3_close(db);

}		


/************************************************** New Change Log ***********************************************************
6/12 - added ability to view an item by its ID. Can now view by item name or ID #, ID # returns only one existing transaction


*/
#include "header.h"
#include "database.h"
#include "raidDump.h"


int main() {
	string readFrom[10] = { "raidDump.txt", "raidDump2.txt", "raidDump3.txt" , "raidDump4.txt" ,
						   "raidDump5.txt" , "raidDump6.txt" , "raidDump7.txt" , "raidDump8.txt" ,
						   "raidDump9.txt" , "raidDump10.txt" };

	database raidDump;
	string outFile = "RaidData.txt";
	string input;



	//raidDump.update();
	raidDump.mainMenu();

















	//cout << "Enter the Date: " << endl;
	//cin >> input;

	

	return 0;
}






//int date;
//int check;



//char *sql;
//char *zErrMsg = 0;
//sqlite3 *db;


//command = "CREATE TABLE ITEM(" 
//"NAME    TEXT NOT NULL," 
//"BUYER   TEXT," 
//"COST    INT," 
//"DATE    INT);";


//sql ="CREATE TABLE PLAYER(" \
	//	 "NAME   TEXT PRIMARY KEY  NOT NULL UNIQUE," \
	//	 "LEVEL  INT               NOT NULL," \
	//	 "CLASS  TEXT              NOT NULL," \
	//	 "DKP    INT			   NOT NULL);";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

//sql = "INSERT INTO PLAYER(name,level,class,dkp) VALUES ('Xhann', 70, 'Berserker', 250);";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

//sql = "INSERT INTO PLAYER(name,level,class,dkp) VALUES ('Ythera', 70,' Necromancer', 300);";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

//sql = "ALTER TABLE PLAYER ADD COLUMN ATTENDANCE FLOAT;";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

//sql = "ALTER TABLE PLAYER ADD COLUMN ;";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);


//sql = "SELECT * FROM PLAYER;";

//check = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

//for(int index = 0; index < 10; index++)
//{
//parse(readFrom[index], raidDump, date);
//}
//cout << "Pre-Print Raid Total: " << raidDump.raidTotal() << endl << endl;
//raidDump.printRaid(outFile);
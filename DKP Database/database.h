#pragma once
#include<iostream>
#include<string>
#include<math.h>
#include<fstream>
#include<list>
#include "sqlite3.h"
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
//#include <Commdlg.h>
using namespace std;

//callback function declarations
int callback(void *data, int argc, char **argv, char **azColName);
int callbackitem(void *data, int argc, char **argv, char **azColName);
int callbackraid(void *data, int argc, char **argv, char **azColName);
int callbackdump(void *data, int argc, char **argv, char **azColName);
int callbackshit(void *data, int argc, char **argv, char **azColName);
int callbackno(void *data, int argc, char **argv, char **azColName);
int callbackremovedump(void *data, int argc, char **argv, char **azColName);
int callbacksingle(void *data, int argc, char **argv, char **azColName);
int callbackplayerinfo(void *data, int argc, char **argv, char **azColName);
int callbackdumpsummary(void *data, int argc, char **argv, char **azColName);



struct purchase 
{
	string logLine = "";
	string itemName = "";
	string buyerName = "";
	string secondBuyer = "";
	string buyerType = "";
	int dkpVal = 0;
	bool complete = 0;
	string errorFlag = "";
};

purchase parseLootLine(string lootLine,sqlite3 *db);

string findWord(string stringLine, int &place);
string findWord(string stringLine, int &place, bool changeCase);

string findItem(string stringLine, int &place);
string peekWord(string stringLine, int place);
string peekWord(string stringLine, int place, bool changeCase);

bool   exists(string table, string column, string value, sqlite3* db, int print);
bool   exists(string table, string column, int    value, sqlite3* db, int print);

string findDBVal(string value, string table, string column, string known, sqlite3* db);
string findDBVal(string value, string table, string column, int    known, sqlite3* db);

string formColumn(string word, int columnSize);
string formColumn(int  &dkpVal, int columnSize);

string getName();
int    getDKP();
string getItem();
int    getDate(string &dateForm);

bool isNum(string test);

class database
{
public:
	//Functions
	void update();
	void mainMenu();

	void accessPlayer();
	void accessItem();
	void accessRaid();
	void accessAdmin();

	void addPlayer();
	void addPlayer(string newName);
	bool addPlayer(string newName, string newClass, int newLevel);
	void addItem();
	void addItem(int date, string buyer, string item, int dkpVal);
	void addFile();

	void removePlayer();
	void removeItem();
	void removeRaid();


	void viewPlayer();
	void viewItem();
	void viewRaid();
	void viewDumps();

	void updatePlayer();
	void updateItem();
	void updateRaid();


	void printPlayers();
	void printItems();
	void printItem(string name);
	void printItem(int ID);
	void printItems(int date);

	void printRaids();
	void printRaid(int date);



	void printPlayer(string name);
	void printPlayerItems(string name);
	void printPlayerRaids(string name);

	void printDumps();
	void printDumps(int date);
	void printDumpSummary(int date);

	void parseDump(string directory,string fileName[],int fileNum);
	void revertDump(int dateVal);
	void parseLoots(string directory, string fileName[], int fileNum);

	//Incrementers/Decrementers
	//**************************
	void incPlayerCount();
	void decPlayerCount();
	void incItemCount();
	//**************************


	database();
	~database();

private:

	int playerCount;
	int itemCount;	
	int raidCount;

	char *zErrMsg = 0;
	const char *cErrMsg = 0;
	sqlite3_stmt *stmt;
	sqlite3 *db;
	int check;
	char* sql;
	const char* sqlC;
	const char* arg1;
	const char* arg2;
	const char* arg3;

	string stats = "C:\\Users\\Kyle\\source\\repos\\DKP Program\\SumCodeMang\\stats.txt";
	
};

//sql = "SELECT sql FROM sqlite_master WHERE name = 'ITEM';"; -- to return table specific info
//sql = "SELECT name FROM sqlite_master WHERE type = 'table';"; -- to list the tables in database


//sql = "CREATE TABLE PLAYER(NAME TEXT NOT NULL UNIQUE,LEVEL INT NOT NULL,CLASS TEXT,TYPE TEXT,DKP INT NOT NULL,DKPTOTAL INT NOT NULL,ATTENDANCE FLOAT NOT NULL,LIFETIME FLOAT NOT NULL,CHECKS INT NOT NULL, CHECKSTOTAL INT NOT NULL);";
//sql = "INSERT INTO PLAYER(name,level,class,type,dkp,dkptotal,attendance,lifetime,checks,checkstotal) VALUES ('Xhann', 70, 'Berserker', 'Main', 0,0,0,0,0,0);";

//sql = "CREATE TABLE RAID(DATE INT UNIQUE,ITEMS INT,DKPEARNED INT,RAIDERS INT,RAIDERSAVG FLOAT, DKPSPENT INT, DKPRAID INT,DUMPS INT);";
//sql = "INSERT INTO RAID(DATE,ITEMS,DKPEARNED,RAIDERS,RAIDERSAVG, DKPSPENT, DKPRAID,DUMPS INT) VALUES (12202017, 47, 158, 52, 48.5, 7425, 8216,5);";

//sql = "CREATE TABLE RAIDDUMP(DATE INT,TIME INT,EVENT TEXT,PLAYER TEXT,DKP INT,FILE TEXT);";
//sql = "INSERT INTO RAIDDUMP(DATE,TIME,EVENT,PLAYER,DKP,FILE) VALUES (12312017, 2130, 'Overlord Mata Muram', 'Xhann', 5, 'test.txt');";

//sql = "CREATE TABLE ITEM(ID INT UNIQUE, DATE INT,BUYER TEXT,ITEM TEXT,SPENT INT);";
//sql = "INSERT INTO ITEM(ID,DATE,BUYER,ITEM,SPENT) VALUES (1, 12202017, 'Venalin', 'Morguecaller', 50);";

//sql = "CREATE TABLE FILECHECK(FILE TEXT UNIQUE);";
//sql = "INSERT INTO FILECHECK(FILE) VALUES ('test.txt');";

//*****To clear the data from a table*****
//sql = "DELETE FROM ITEM;";
//check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
//sql = "VACUUM;";
//check = sqlite3_exec(db, sql, callbackdump, 0, &zErrMsg);
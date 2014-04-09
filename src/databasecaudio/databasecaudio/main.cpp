#include <stdlib.h>
#include <stdio.h>
#include <io.h>
#include <Windows.h>
#include <string>
#include "ahm.h"
#include <iostream>
#include <fileref.h>
#include <tag.h>
#include <tpropertymap.h>
#include "../sqlite3.h"
#include <dirent.h>
using namespace std;
using namespace TagLib;

string directory("C:\\Users\\RazLT\\Music");

int main(void)
{
	int rc;
	char *error;
	cout << "Opening musiclib.db ..." << endl;
	sqlite3 *db;
	rc = sqlite3_open("musiclib.db", &db);
	if (rc)
	{
		cerr << "Error opening SQLite3 database: " << sqlite3_errmsg(db) << endl << endl;
		sqlite3_close(db);
		return 1;
	}
	else
	{
		cout << "Opened musiclib.db." << endl << endl;
	}
	createDirectory();
	
	//FileRef f(directory + "2025 (Holy Other Remix).wav");
	//Tag *tag = f.tag();
	//cout << tag->title();
	//const char *sqlCreateTable = "CREATE TABLE MyTable (id INTEGER PRIMARY KEY, value STRING);";
	//rc = sqlite3_exec(db, sqlCreateTable, NULL, NULL, &error);
	//if (rc)
	//{
	//	cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
	//	sqlite3_free(error);
	//}
	//else
	//{
	//	cout << "Created MyTable." << endl << endl;
	//}

	//// Execute SQL
	//cout << "Inserting a value into MyTable ..." << endl;
	//const char *sqlInsert = "INSERT INTO MyTable VALUES(NULL, 'A Value');";
	//rc = sqlite3_exec(db, sqlInsert, NULL, NULL, &error);
	//if (rc)
	//{
	//	cerr << "Error executing SQLite3 statement: " << sqlite3_errmsg(db) << endl << endl;
	//	sqlite3_free(error);
	//}
	//else
	//{
	//	cout << "Inserted a value into MyTable." << endl << endl;
	//}

	//// Display MyTable
	//cout << "Retrieving values in MyTable ..." << endl;
	//const char *sqlSelect = "SELECT * FROM MyTable;";
	//char **results = NULL;
	//int rows, columns;
	//sqlite3_get_table(db, sqlSelect, &results, &rows, &columns, &error);
	//if (rc)
	//{
	//	cerr << "Error executing SQLite3 query: " << sqlite3_errmsg(db) << endl << endl;
	//	sqlite3_free(error);
	//}
	//else
	//{
	//	// Display Table
	//	for (int rowCtr = 0; rowCtr <= rows; ++rowCtr)
	//	{
	//		for (int colCtr = 0; colCtr < columns; ++colCtr)
	//		{
	//			// Determine Cell Position
	//			int cellPosition = (rowCtr * columns) + colCtr;

	//			// Display Cell Value
	//			cout.width(12);
	//			cout.setf(ios::left);
	//			cout << results[cellPosition] << " ";
	//		}

	//		// End Line
	//		cout << endl;

	//		// Display Separator For Header
	//		if (0 == rowCtr)
	//		{
	//			for (int colCtr = 0; colCtr < columns; ++colCtr)
	//			{
	//				cout.width(12);
	//				cout.setf(ios::left);
	//				cout << "~~~~~~~~~~~~ ";
	//			}
	//			cout << endl;
	//		}
	//	}
	//}
	//sqlite3_free_table(results);

	//// Close Database
	//cout << "Closing MyDb.db ..." << endl;
	//sqlite3_close(db);
	//cout << "Closed MyDb.db" << endl << endl;

	//// Wait For User To Close Program
	//cout << "Please press any key to exit the program ..." << endl;
	cin.get();

	return 0;
}

int createDirectory()
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (directory.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			printf ("%s\n", ent->d_name);
		}
		closedir (dir);
	} else {
		/* could not open directory */
		perror ("");
		return EXIT_FAILURE;
	}
}
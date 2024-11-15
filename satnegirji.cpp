#include <sqlite3.h>
#include <iostream>
#include <cstdlib>
#include "utils.h"
using namespace std;

//
// This work is licensed under
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International.
// To view a copy of this license,
// please visit https://creativecommons.org/licenses/by-nc-sa/4.0/
// Dedicated to the memory of Kristian "Rico" Guttorm.
// To compile, run:
// g++ -o satnegirji satnegirji.cpp utils.cpp -lsqlite3
// 

string userInput;

void replace_all(
std::string& userInput,
std::string const& toReplace,
std::string const& replaceWith
) {
std::string buf;
std::size_t pos = 0;
std::size_t prevPos;

// Reserves rough estimate of final size of string.
buf.reserve(userInput.size());

while (true) {
prevPos = pos;
pos = userInput.find(toReplace, pos);
if (pos == std::string::npos)
break;
buf.append(userInput, prevPos, pos - prevPos);
buf += replaceWith;
pos += toReplace.size();
}

buf.append(userInput, prevPos, userInput.size() - prevPos);
userInput.swap(buf);
}

Utils utils;

int main() {

	// When running on Windows, change the codepage to 65001 ie UTF-8
	// Windows is using UTF-16 as default
	// Compiling this on Windows, you must use g++ as it is UTF-8 compatible
	// On Windows, use Mingw-w64/MSYS2 g++ compiler

	#ifdef _WIN32
		system( "chcp 65001 >null" );
	#endif

	sqlite3* db;
	//char *zErrMsg = 0;
	int rc;
	string sql;

	string choise;
	bool choiseDone;
	choiseDone = false;

	int quit = 0;
	quit = sqlite3_open("satnegirji.db", &db);

	if (quit) {

		cerr << "SQLite: " << sqlite3_errmsg(db) << "\n";

	} else {

		utils.clearScreen();

	}
	
	while (true) {

		if (choiseDone == false || userInput == "??") {

			cout << "0 - Sulje ohjelma - Gidde prográmma\n";
			cout << "1 - Haku suomen kielellä\n";
			cout << "2 - Oza sámegillii\n";
			cout << "3 - Kokotekstihaku - Ollesdeakstaohcu\n";
			cout << "?? - Hakuoptiot - Ohcumolssaeavttut\n";
			cout << "Hakuoptio? - Ohcumolssaeaktu?>> ";
			getline(cin, choise);

		}

		if (choise == "0") {

			utils.clearScreen();
			sqlite3_close(db);
			exit(0);

} else if (choise == "3") {

			// Search from sámi and finnish

			choiseDone = true;

			cout << "Kokotekstihaku - Ollesdeakstaohcu>> ";
			getline(cin, userInput);
		
			replace_all(userInput, ";", "");
			replace_all(userInput, "*", "%");

			sqlite3_stmt *stmt;

			if (!userInput.empty()) { 

    			sql = "SELECT DISTINCT suomi as 'Suomi',saame as 'Sápmi' FROM suomisaame WHERE suomisaame match ?  AND rank MATCH 'bm25(10.0, 5.0)' ORDER BY rank limit 1000;";
    			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
			
    		}
			
			if (rc != SQLITE_OK) {
        		
				// handle error
        		std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;

    		} else {

        		sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL);  // bind user input
        		
				while (sqlite3_step(stmt) == SQLITE_ROW) {  // execute prepared statement
            		const unsigned char *suomi = sqlite3_column_text(stmt, 0);
            		const unsigned char *saame = sqlite3_column_text(stmt, 1);
            		std::cout << "Suomi: " << suomi << "\nSápmi: " << saame << std::endl;
					cout << "\n";
        		}

        	sqlite3_finalize(stmt);  // cleanup

    		}

		} else if (choise == "2") {

			// Search from sámi to finnish

			choiseDone = true;

			cout << "Oza sámegillii>> ";
			getline(cin, userInput);
		
			replace_all(userInput, ";", "");
			replace_all(userInput, "*", "%");

			sqlite3_stmt *stmt;

			if (!userInput.empty()) { 

    			sql = "SELECT DISTINCT suomi as 'Suomi',saame as 'Sápmi' FROM skirja WHERE saame LIKE ? order by saame limit 1000;";
    			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
			
    		}
			
			if (rc != SQLITE_OK) {
        		
				// handle error
        		std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;

    		} else {

        		sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL);  // bind user input
        		
				while (sqlite3_step(stmt) == SQLITE_ROW) {  // execute prepared statement
            		const unsigned char *suomi = sqlite3_column_text(stmt, 0);
            		const unsigned char *saame = sqlite3_column_text(stmt, 1);
            		std::cout << "Suomi: " << suomi << "\nSápmi: " << saame << std::endl;
					cout << "\n";
        		}

        	sqlite3_finalize(stmt);  // cleanup

    		}

		} else if (choise == "1") {
			
			// Search from finnish to sámi

			choiseDone = true;

			cout << "Haku suomeksi>> ";
			getline(cin, userInput);
		
			replace_all(userInput, ";", "");
			replace_all(userInput, "*", "%");

			sqlite3_stmt *stmt;
			
			if (!userInput.empty()) { 

    			sql = "SELECT DISTINCT suomi as 'Suomi',saame as 'Sápmi' FROM skirja WHERE suomi LIKE ? order by suomi limit 1000;";
    			rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
			
    		}
			
			if (rc != SQLITE_OK) {

        		// handle error
        		std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;

    		} else {

        		sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL);  // bind user input
        		
				while (sqlite3_step(stmt) == SQLITE_ROW) {  // execute prepared statement
            		
					const unsigned char *suomi = sqlite3_column_text(stmt, 0);
            		const unsigned char *saame = sqlite3_column_text(stmt, 1);
            		std::cout << "Suomi: " << suomi << "\nSápmi: " << saame << std::endl;
					cout << "\n";

        		}

        	sqlite3_finalize(stmt);  // cleanup
    		}

		} else if (choise =="??" || userInput == "??") {

			choiseDone = false;

		}
	}

	sqlite3_close(db);
	return (0);
}
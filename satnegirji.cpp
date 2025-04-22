#include <sqlite3.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <regex>
#include <algorithm>
#include "utils.h"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#include <limits.h>
#elif __APPLE__
#include <mach-o/dyld.h>
#include <limits.h>
#endif

using namespace std;

//
// This work is licensed under
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International.
// To view a copy of this license,
// please visit https://creativecommons.org/licenses/by-nc-sa/4.0/
// Dedicated to the memory of Kristian "Rico" Guttorm (1972-1999).
//

// Function to sanitize input for search
void sanitizeInputForSearch(std::string &input)
{
    // Define the pattern for allowed characters, including '?'
    std::regex pattern("[^a-zA-ZåäöčđŋšŧžáøæéàüÅÄÖČĐŊŠŦŽÁØÆÉÀÜ*\'-?]");
    // Remove all characters not in the allowed set
    input = std::regex_replace(input, pattern, "");
}

// Function to replace user-submitted * wildcard with SQL-compatible % character
void enableWildcard(std::string &input)
{
    size_t pos = 0;
    while ((pos = input.find("*", pos)) != std::string::npos)
    {
        input.replace(pos, 1, "%");
        pos += 1; // Move past the replacement
    }
}

// Function to remove user-submitted * wildcard and SQL-compatible % character
void disableWildcard(std::string &input)
{
    input.erase(std::remove(input.begin(), input.end(), '*'), input.end());
    input.erase(std::remove(input.begin(), input.end(), '%'), input.end());
}

// Function to escape double quotes for FTS5
void escapeForFTS5(std::string &input)
{
    std::string result = "\"";
    for (char c : input)
    {
        if (c == '"')
        {
            result += "\"\""; // Escape double quotes
        }
        else
        {
            result += c;
        }
    }
    result += "\"";
    input = result;
}

// Function to get the executable's directory
std::string getExecutablePath()
{
    char result[PATH_MAX];

#ifdef _WIN32
    // Windows: Use GetModuleFileName to get the executable path
    if (GetModuleFileNameA(NULL, result, MAX_PATH))
    {
        std::string fullPath(result);
        size_t lastSlash = fullPath.find_last_of('\\');
        return fullPath.substr(0, lastSlash); // Return the directory path
    }
#elif __linux__
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    if (count != -1)
    {
        std::string fullPath(result, count);
        size_t lastSlash = fullPath.find_last_of('/');
        return fullPath.substr(0, lastSlash); // Return the directory path
    }
#elif __APPLE__
    uint32_t size = sizeof(result);
    if (_NSGetExecutablePath(result, &size) == 0)
    {
        std::string fullPath(result);
        size_t lastSlash = fullPath.find_last_of('/');
        return fullPath.substr(0, lastSlash); // Return the directory path
    }
#endif
    return ""; // Failed to determine the path
}

string userInput;

Utils utils;

int main()
{

    // When running on Windows, change the codepage to 65001 ie UTF-8
    // Windows is using UTF-16 as default
    // Compiling this on Windows, you must use g++ as it is UTF-8 compatible
    // On Windows, use Mingw-w64/MSYS2 g++ compiler

#ifdef _WIN32
    system("chcp 65001 >null");
#endif

    sqlite3 *db;
    // char *zErrMsg = 0;
    int rc;
    string sql;

    string choise;
    bool choiseDone;
    choiseDone = false;

    int quit = 0;

    // Get the directory of the executable
    string executableDir = getExecutablePath();
    if (executableDir.empty())
    {
        cerr << "Failed to determine executable directory." << endl;
        return 1;
    }

    // Construct the full path to the database file
    string dbPath = executableDir + "/satnegirji.db";

    // Open the database
    quit = sqlite3_open(dbPath.c_str(), &db);
    if (quit != SQLITE_OK)
    {
        cerr << "Cannot open database: " << sqlite3_errmsg(db) << endl;
        return quit;
    }
    // Uncomment the line below and define the directtory by yourself to use a hardcoded path instead
    // quit = sqlite3_open("/Users/tryhanen/Documents/satnegirji/code/c-plusplus/satnegirji.db", &db);

    if (quit)
    {

        cerr << "SQLite: " << sqlite3_errmsg(db) << "\n";
    }
    else
    {

        utils.clearScreen();
    }

    while (true)
    {

        // Debugging output to track state
        // std::cout << "DEBUG: choise = " << choise << ", userInput = " << userInput << std::endl;
        //std::cout << "DEBUG: choise = " << choise
        //          << ", userInput = " << userInput
        //          << ", choiseDone = " << (choiseDone ? "true" : "false")
        //          << std::endl;

        if (choiseDone == false || userInput == "??")
        {

            cout << "0 - Sulje ohjelma - Gidde prográmma\n";
            cout << "1 - Haku suomen kielellä\n";
            cout << "2 - Oza sámegillii\n";
            cout << "3 - Kokotekstihaku - Ollesdeakstaohcu\n";
            cout << "?? - Hakuoptiot - Ohcumolssaeavttut\n";
            cout << "Hakuoptio? - Ohcumolssaeaktu?>> ";
            getline(cin, choise);
        }

        if (choise == "0")
        {

            utils.clearScreen();
            sqlite3_close(db);
            exit(0);
        }
        else if (choise == "3")
        {
            choiseDone = true; // Stay inside this mode

            while (true)
            { // Infinite loop for continuous searching until "??"
                cout << "Kokotekstihaku - Ollesdeakstaohcu>> ";
                getline(cin, userInput);

                if (userInput == "??")
                {
                    choise = ""; // Reset to return to search options
                    choiseDone = false;
                    userInput = "";
                    utils.clearScreen();
                    break; // Exit the inner loop and return to the main menu
                }

                sanitizeInputForSearch(userInput);
                disableWildcard(userInput);
                escapeForFTS5(userInput);

                sqlite3_stmt *stmt;

                if (!userInput.empty())
                {
                    sql = "SELECT DISTINCT suomi as 'Suomi', saame as 'Sápmi' FROM suomisaame WHERE suomisaame match ? AND rank MATCH 'bm25(10.0, 5.0)' ORDER BY rank LIMIT 1000;";
                    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
                }

                if (rc != SQLITE_OK)
                {
                    cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << endl;
                }
                else
                {
                    sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL);

                    while (sqlite3_step(stmt) == SQLITE_ROW)
                    {
                        const unsigned char *suomi = sqlite3_column_text(stmt, 0);
                        const unsigned char *saame = sqlite3_column_text(stmt, 1);
                        cout << "Suomi: " << suomi << "\nSápmi: " << saame << "\n"
                             << endl;
                    }

                    sqlite3_finalize(stmt);
                }
            }
        }
        else if (choise == "2")
        {

            // Search from sámi to finnish

            choiseDone = true;

            cout << "Oza sámegillii>> ";
            getline(cin, userInput);

            sanitizeInputForSearch(userInput);
            enableWildcard(userInput);

            sqlite3_stmt *stmt;

            if (!userInput.empty())
            {

                sql = "SELECT DISTINCT suomi as 'Suomi',saame as 'Sápmi' FROM skirja WHERE saame LIKE ? order by saame limit 1000;";
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
            }

            if (rc != SQLITE_OK)
            {

                // handle error
                std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;
            }
            else
            {

                sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL); // bind user input

                while (sqlite3_step(stmt) == SQLITE_ROW)
                { // execute prepared statement
                    const unsigned char *suomi = sqlite3_column_text(stmt, 0);
                    const unsigned char *saame = sqlite3_column_text(stmt, 1);
                    std::cout << "Suomi: " << suomi << "\nSápmi: " << saame << std::endl;
                    cout << "\n";
                }

                sqlite3_finalize(stmt); // cleanup
            }
        }
        else if (choise == "1")
        {

            // Search from finnish to sámi

            choiseDone = true;

            cout << "Haku suomeksi>> ";
            getline(cin, userInput);

            sanitizeInputForSearch(userInput);
            enableWildcard(userInput);

            sqlite3_stmt *stmt;

            if (!userInput.empty())
            {

                sql = "SELECT DISTINCT suomi as 'Suomi',saame as 'Sápmi' FROM skirja WHERE suomi LIKE ? order by suomi limit 1000;";
                rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
            }

            if (rc != SQLITE_OK)
            {

                // handle error
                std::cerr << "Failed to prepare SQL: " << sqlite3_errmsg(db) << std::endl;
            }
            else
            {

                sqlite3_bind_text(stmt, 1, userInput.c_str(), -1, NULL); // bind user input

                while (sqlite3_step(stmt) == SQLITE_ROW)
                { // execute prepared statement

                    const unsigned char *suomi = sqlite3_column_text(stmt, 0);
                    const unsigned char *saame = sqlite3_column_text(stmt, 1);
                    std::cout << "Suomi: " << suomi << "\nSápmi: " << saame << std::endl;
                    cout << "\n";
                }

                sqlite3_finalize(stmt); // cleanup
            }
        }
        else if (choise == "??" || userInput == "??")
        {
            choise = "";
            choiseDone = false;
            userInput = "";
            utils.clearScreen();
            continue;
        }
    }

    sqlite3_close(db);
    return (0);
}
//
// This work is licensed under
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International.
// To view a copy of this license,
// please visit https://creativecommons.org/licenses/by-nc-sa/4.0/
// Dedicated to the memory of Kristian "Rico" Guttorm.
// To compile, run:
// g++ -o satnegirji satnegirji.cpp utils.cpp -lsqlite3
//

#include "utils.h"
#include <cstdlib>

void Utils::clearScreen() {
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

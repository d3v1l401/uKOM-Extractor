/*
		This file is part of uKOM Extractor.

		uKOM Extractor is free software : you can redistribute it and / or modify
		it under the terms of the GNU General Public License as published by
		the Free Software Foundation, either version 3 of the License, or
		(at your option) any later version.

		uKOM Extractor is distributed in the hope that it will be useful,
		but WITHOUT ANY WARRANTY; without even the implied warranty of
		MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
		GNU General Public License for more details.

		You should have received a copy of the GNU General Public License
		along with uKOM Extractor. If not, see <http://www.gnu.org/licenses/>.

		----------------------------------------------------------------------------

		Made by d3vil401 (http://d3vsite.org)
		Give credits when using part of this code or it helped you (or i will stop releasing, new LUA decryption C code too c:)
*/

#include <iostream>
#include <advconsole.h>
#include "Global.h"
#include <conio.h>
#include "ATaker.h"
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;

void main(short int argc, const char* argv[])
{
	SetConsoleTitle("uKOM Archiver - PUBLIC");
	//printf(ENTRY_BANNER, __DATE__, __TIME__);
	cout << Color(AC_CYAN, AC_BOLD) << ENTRY_BANNER << __DATE__ << " " << __TIME__ << "\n";
	cout << Color(AC_GREEN) << "uKOM Extractor is a free software under GPLv3 license.\nGive credits to the original author whenever you use part of the code or it helped you.\n\n";
	ATaker::AssumeParameters(argc, argv);
	ATaker::Initialize();

	cout << Color(AC_GREEN) << "All done.\n" << Color(AC_WHITE);
	_getch();
}
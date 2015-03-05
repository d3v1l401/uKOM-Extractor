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

#include "ATaker.h"
#include "HManager.h"
//#include <advconsole.h>
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;
#pragma warning (disable: 4996)

#define ARG_ACTION		1
#define ARG_KOMDIRNAME	2
#define ARG_FILTER		3
#define ARG_FILTERVALUE 4

char* ATaker::ActionType  = nullptr;
char* ATaker::TargetName  = nullptr;
char* ATaker::FilterType  = nullptr;
char* ATaker::FilterValue = nullptr;
short int ATaker::Argc = NULL;

const char* ATaker::HelpMessage = "[ HELP ]\n\n"
								  "- COMMANDS -\n\n"
								  "-h: Will print this help message.\n"
								  "-u: Will unpack the selected KOM.\n"
								  "-p: Will pack the selected folder (Required KOMInfo.xml into it).\n";
								 /* uKOM Extractor (FREE Open Source) does not support these. :)
								  "\n- OPTIONAL FILTER -\n\n"
								  "-file: Will extract the specifyed file.\n"
								  "-ext: Will extract the files with the specifyed extensione (lua, x, tet, y, tga, txt, ini).\n"
								  "-alg: Will extract the files with the specifyed algorithm (0, 1, 2).\n"
								  "\n- COMMAND LINE STRUCTURE -\n\n"
								  "uKOMArchiver.exe [COMMAND] [KOM/FOLDER NAME] [OPT: FILTER TYPE] [OPT: FILTER VALUE]\n\n";
								  */

void ATaker::AssumeParameters(short int argc, const char* argv[])
{
	if (!argv)
		cout << Color(AC_RED) << "No parameters parsed in AssumeParameters.\n";
	if (!argc)
		cout << Color(AC_RED) << "No parameters number parsed in AssumeParameters.\nPlease use -h if you don't know how to use uKOM Archiver.\n";
	
	switch (argc)
	{
	case 2:
		cout << Color(AC_YELLOW) << (char*)ATaker::HelpMessage;
		break;
	case 3:
		ATaker::Argc = 3;
		ATaker::ActionType = (char*)argv[ARG_ACTION];
		ATaker::TargetName = (char*)argv[ARG_KOMDIRNAME];
		cout << Color(AC_WHITE) << "Selected Target: " << Color(AC_GREEN) << ATaker::TargetName << "\n";
		break;
#ifdef PRIVATE_VERSION
	case 5:
		ATaker::Argc = 5;
		ATaker::ActionType  = (char*)argv[ARG_ACTION];
		ATaker::TargetName  = (char*)argv[ARG_KOMDIRNAME];
		ATaker::FilterType  = (char*)argv[ARG_FILTER];
		ATaker::FilterValue = (char*)argv[ARG_FILTERVALUE];
		cout << Color(AC_WHITE) << "Selected Target: " << Color(AC_GREEN) << ATaker::TargetName << "\n" 
			 << Color(AC_WHITE) << "With filter " << Color(AC_GREEN) << ATaker::FilterType 
			 << Color(AC_WHITE) << " as " << Color(AC_GREEN) << ATaker::FilterValue << "\n";
		break;
#endif
	default:
		cout << Color(AC_RED) << "No parameters number parsed in AssumeParameters.\n" << Color(AC_YELLOW) << "Please use -h if you don't know how to use uKOM Archiver.\n";
		break;
	}
}

void ATaker::Initialize()
{
	switch (ATaker::Argc)
	{
	case 3:
		if (!strcmp(ATaker::ActionType, "-u"))
		{
			HManager::Init(ACTION_UNPACK, ATaker::TargetName);
			break;
		}
		if (!strcmp(ATaker::ActionType, "-p"))
		{
			HManager::Init(ACTION_PACK, ATaker::TargetName);
			break;
		}
		else {
			cout << Color(AC_RED) << "Unrecognized parameter parsed in < COMMAND > position.\n";
			break;
		}
		break;
#ifdef PRIVATE_VERSION
	case 5:
		if (!strcmp(ATaker::ActionType, "-u"))
		{
			if (!strcmp(ATaker::FilterType, "-file"))
			{
				HManager::Init(ACTION_UNPACK, ATaker::TargetName, FILTER_TYPE_FILE, ATaker::FilterValue);
				break;
			}
			if (!strcmp(ATaker::FilterType, "-ext"))
			{
				HManager::Init(ACTION_UNPACK, ATaker::TargetName, FILTER_TYPE_EXT, ATaker::FilterValue);
				break;
			}
			if (!strcmp(ATaker::FilterType, "-alg"))
			{
				HManager::Init(ACTION_UNPACK, ATaker::TargetName, FILTER_TYPE_ALG, ATaker::FilterValue);
				break;
			}
			else {
				cout << Color(AC_RED) << "Unrecognized parameter parsed in < FILTER TYPE > position.\n";
				break;
			}
			break;
		}
		if (!strcmp(ATaker::ActionType, "-p"))
		{
			cout << Color(AC_YELLOW) << "Packing does not need filters.\nIf you need to edit KOM content then you need to edit KOMInfo.xml in the path.\nIgnoring Filter.\n";
			HManager::Init(ACTION_PACK, ATaker::TargetName);
			break;
		}
		else {
			cout << Color(AC_RED) << "Unrecognized parameter parsed in < COMMAND > position.\n";
			break;
		}
		break;
#endif
	default:
		cout << Color(AC_RED) << "No commands to initialize.\n";
		break;
	}
}
#pragma once

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
#include <Windows.h>
#include <string>
using namespace std;

class ATaker
{
private:
	
	static char* TargetName;
	static char* ActionType;
	static char* FilterType;
	static char* FilterValue;
	static short int Argc;
	

	static const char* HelpMessage;

protected:


public:

	static void AssumeParameters(short int argc, const char* argv[]);
	static void Initialize();

};
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
#include <conio.h>
#include <vector>
#include <advconsole.h>
#include "XMLManager.h"
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;

class EManager
{
public: 
	static class ZLib
	{
	private:



	protected:


	public:

		static int Decompress(char* Output, unsigned long OutSize, char* Input, unsigned long InSize);

	};

	static class Crypto
	{
	private:
#ifdef PRIVATE_VERSION
		static DWORD GenerateKp();
#endif
	protected:


	public:
#ifdef PRIVATE_VERSION
		static void DecryptLUA(char* Input, unsigned long Size, char* Output);
		static void EncryptLUA(char* Input, unsigned long Size, char* Output);
#endif
	};
private:

	static char* CurrentPtr;
	static char* Buff;
	static char* KOMName;

	static void Save(char* FileName, char* FileBuff, unsigned long FileSize);
protected:


public:

	static void Init(char* Buffer, unsigned long BSize, vector<XMLManager::_XML_ENTRY> FilesList, unsigned long XMLSize, char* TargetName);
	static void Extract(vector<XMLManager::_XML_ENTRY> FilesList);
};
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

#include "EManager.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>
//#include <zlib.h>
#include <vector>
#include "miniz.c"
#pragma warning (disable: 4996)
//#pragma comment(lib, "zdll.lib")
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

/*

	Thanks to Joni-st which remembered me the types (and gave me notice i lost the type 2 that has been used 2 weeks).

*/
#define ALGORITHM_COMPRESSED 0
#define ALGORITHM_NOTUSED	 1
#define ALGORITHM_COMP_ENCR	 2
#define ALGORITHM_ENCR_COMP	 3

#define HEADER_SIZE 72

char* EManager::CurrentPtr = NULL;
char* EManager::Buff = nullptr;
char* EManager::KOMName = nullptr;

int EManager::ZLib::Decompress(char* Output, unsigned long OutSize, char* Input, unsigned long InSize)
{
	int zr = uncompress((unsigned char*)Output, &OutSize, (const unsigned char*)Input, InSize);
	switch (zr)
	{
	case Z_OK:
		return Z_OK;
		break;
	case Z_MEM_ERROR:
		cout << Color(AC_RED) << "Not enough memory for decompression.\n";
		return Z_MEM_ERROR;
		break;
	}
	return zr;
}

void EManager::Save(char* FileName, char* FileBuff, unsigned long FileSize)
{
	char* FinalPath[MAX_PATH] = { 0 };

	FILE* TmpFile = fopen((char*)FileName, "wb");
	if (!TmpFile)
	{
		cout << Color(AC_RED) << "Cannot save the extracted file " << FileName << "!\n";
		_getch();
		exit(0);
	}
	fwrite(FileBuff, sizeof(char), FileSize, TmpFile);
	fclose(TmpFile);
}

void EManager::Init(char* Buffer, unsigned long BSize, vector<XMLManager::_XML_ENTRY> FilesList, unsigned long XMLSize, char* TargetName)
{
	EManager::Buff = Buffer;
	EManager::CurrentPtr = ((EManager::Buff + HEADER_SIZE) + XMLSize);
	EManager::KOMName = TargetName;

	EManager::Extract(FilesList);
}

void EManager::Extract(vector<XMLManager::_XML_ENTRY> FilesList)
{
	int ZRes = NULL;
	vector<XMLManager::_XML_ENTRY>::iterator XMLIter = FilesList.begin();
	do
	{
		XMLManager::_XML_ENTRY Entry = *XMLIter;
		char* InputData = (char*)malloc(Entry.CompressedSize);
		char* MiddleData = (char*)malloc(Entry.Size + 1);
		char* OutputData = (char*)malloc(Entry.Size + 1);
		memset(InputData, 0x00, Entry.CompressedSize);
		memset(MiddleData, 0x00, Entry.Size + 1);
		memset(OutputData, 0x00, Entry.Size + 1);

		memcpy(InputData, EManager::CurrentPtr, Entry.CompressedSize);

		cout << Color(AC_WHITE) << "Processing " << Color(AC_GREEN) << Entry.FName
			 << Color(AC_WHITE) << " (" << Color(AC_YELLOW)
			 << Entry.CompressedSize << Color(AC_WHITE) << " -> "
			 << Color(AC_GREEN) << Entry.Size << Color(AC_WHITE)
			 << ") (Alg. " << Color(AC_MAGENTA) << Entry.Algorithm << Color(AC_WHITE) << ")...";

		switch (Entry.Algorithm)
		{
		case ALGORITHM_COMPRESSED:
			ZRes = EManager::ZLib::Decompress(OutputData, Entry.Size, InputData, Entry.CompressedSize);
			if (ZRes != Z_OK)
			{
				EManager::Save(Entry.FName, OutputData, Entry.Size);
				cout << Color(AC_GREEN) << "Done!\n";
			} else {
				cout << Color(AC_RED) << "FAILED (" << ZRes << ")!\n";
			}
			cout << Color(AC_GREEN) << "Done!\n";
			break;
		case ALGORITHM_COMP_ENCR:
#ifdef PRIVATE_VERSION
			EManager::ZLib::Decompress(MiddleData, Entry.Size, OutputData, Entry.CompressedSize);
			EManager::Crypto::DecryptLUA(MiddleData, Entry.CompressedSize, OutputData);
			EManager::Save(Entry.FName, OutputData, Entry.Size);
#else
			cout << Color(AC_RED) << "Algorithm 2 is partially supported, saving decompressed encrypted data.\n";
			EManager::ZLib::Decompress(OutputData, Entry.Size, InputData, Entry.CompressedSize);
			EManager::Save(Entry.FName, OutputData, Entry.CompressedSize);
#endif
			cout << Color(AC_GREEN) << "Done!\n";
			break;
			break;
		case ALGORITHM_ENCR_COMP:
#ifdef PRIVATE_VERSION
			EManager::Crypto::DecryptLUA(InputData, Entry.CompressedSize, MiddleData);
			EManager::ZLib::Decompress(OutputData, Entry.Size, MiddleData, Entry.CompressedSize);
			EManager::Save(Entry.FName, OutputData2, Entry.Size);
#else
			cout << Color(AC_RED) << "Algorithm 3 is still not supported, saving raw encrypted data.\n";
			EManager::Save(Entry.FName, OutputData, Entry.CompressedSize);
#endif
			cout << Color(AC_GREEN) << "Done!\n";
			break;
		}

		free(InputData);
		free(MiddleData);
		free(OutputData);

		EManager::CurrentPtr += Entry.CompressedSize;
		XMLIter++;
		//FilesList.pop_back();
	} while (XMLIter != FilesList.end());
}
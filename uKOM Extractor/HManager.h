#pragma once
#include <iostream>
#include <vector>
#include <advconsole.h>
#include <string>
#include <conio.h>
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;

#define ACTION_UNPACK		1
#define ACTION_PACK			2

#define FILTER_TYPE_FILE	1
#define FILTER_TYPE_EXT		2
#define FILTER_TYPE_ALG		3

#define HEADER_SIZE			72

#define INT(Array, Offset) (*(reinterpret_cast<unsigned long*>(&Array[Offset])))

class HManager
{
private:

	static FILE* TargetFile;
	static unsigned long TargetSize;
	static char* Buffer;
	static char* TargetName;

#pragma pack(1)
	static struct _KOM_HEADER
	{
		char Header[28];
		unsigned long ArchiverVersion;
		char CRC[5];
		unsigned long XMLSize;
		unsigned long FilesNumber;
		unsigned long CRC2; // [5]
	} KOMHeader;
#pragma

protected:


public:

	static void Init(short int ActionType, char* TargetName);
	static void Init(short int ActionType, char* TargetName, short int FilterType, char* FilterValue);
	static void ReadHeader();
};
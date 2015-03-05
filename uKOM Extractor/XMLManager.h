#pragma once
#include <vector>
#include <Windows.h>
#include <string>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include <sstream>
using namespace std;

class XMLManager
{
private:

	static char* XMLBuff;
	static unsigned long Size;
	static unsigned long XMLSize;
	static char* KOMName;

	static bool CheckFile(char* FileName);
protected:


public:
#pragma pack(1)
	static struct _XML_ENTRY
	{
		char FName[100]; // File name.
		int Size; // Uncompressed file size.
		int CompressedSize; // Compressed file size.
		char Checksum[8]; // EQ FileTime.
		char FileTime[8]; // EQ Checksum.
		int Algorithm; // Algorithm type.
		               //	- Type 0: Nothing.
		               //	- Type 1: Compressed.
					   //	- Type 2: Encrypted & Compressed.
	} Files;
#pragma

	static vector<_XML_ENTRY> FilesList;

	static void Init(char* XMLBuffer, unsigned long XMLSize, unsigned long KOMSize, char* KOMName);
	static void ReadXML();

	static unsigned long GetXML(char* XMLBuffer, unsigned long BuffersSize, unsigned long FNum);

};
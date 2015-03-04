#include "HManager.h"
#include "XMLManager.h"
#pragma warning (disable: 4996)

char* HManager::TargetName			= nullptr;
FILE* HManager::TargetFile			= NULL;
unsigned long HManager::TargetSize	= NULL;
char* HManager::Buffer				= nullptr;
HManager::_KOM_HEADER HManager::KOMHeader;

void HManager::ReadHeader()
{
	if (!HManager::Buffer)
	{
		cout << Color(AC_RED) << "There's nothing in the buffer.\n";
		_getch();
		exit(0);
	}
	strncpy_s(HManager::KOMHeader.Header, HManager::Buffer, 27);
	HManager::KOMHeader.FilesNumber		= INT(HManager::Buffer, 52);
	HManager::KOMHeader.ArchiverVersion = INT(HManager::Buffer, 56);
	strncpy_s(HManager::KOMHeader.CRC, (HManager::Buffer + 60), 4);
	HManager::KOMHeader.CRC2			= INT(HManager::Buffer, 64); // I simply am not sure if they use CRC or another Algorithm, this means i need to re-analyze this part.
	HManager::KOMHeader.XMLSize			= INT(HManager::Buffer, 68);

	// KOG GrandChase Team Massfile Version 0.3.
	// Using a pre-made archive from another game...ES is totally not a GC re-work...KOG...
	if (strcmp(HManager::KOMHeader.Header, "KOG GC TEAM MASSFILE V.0.3."))
	{
		cout << Color(AC_YELLOW) << "Warning: Header mismatch, are you sure this is a KOM? (y/n)\n";
		char Answer = NULL;
		cin >> Answer;
		if (Answer == 'n')
		{
			cout << Color(AC_YELLOW) << "Quitting as ordered...\n";
			exit(1);
		} else {
			cout << Color(AC_YELLOW) << "Proceeding as ordered...\n";
		}
	}

	char AdaptedCRC[10] = { 0 };
	sprintf(AdaptedCRC, "0x%04X", HManager::KOMHeader.CRC);

	cout << Color(AC_WHITE) << "KOM Size: " << Color(AC_GREEN) << HManager::TargetSize
		 << Color(AC_WHITE) << "\nFiles number: " << Color(AC_GREEN) << HManager::KOMHeader.FilesNumber
		 << Color(AC_WHITE) << "\nXML Size: " << Color(AC_GREEN) << HManager::KOMHeader.XMLSize
		 << Color(AC_WHITE) << "\nCRC: " << Color(AC_GREEN) << AdaptedCRC << "\n\n";

	XMLManager::Init(HManager::Buffer, HManager::KOMHeader.XMLSize, HManager::TargetSize, HManager::TargetName);
}

void HManager::Init(short int ActionType, char* TargetName)
{
	if (ActionType != ACTION_PACK)
	{
		HManager::TargetName = TargetName;
		HManager::TargetFile = fopen(TargetName, "rb");
		if (!HManager::TargetFile)
		{
			cout << Color(AC_RED) << "Unable to read " << TargetName << ".\n";
			_getch();
			exit(0);
		}
		fseek(HManager::TargetFile, 0, SEEK_END);
		HManager::TargetSize = ftell(HManager::TargetFile);
		fseek(HManager::TargetFile, 0, SEEK_SET);
		
		HManager::Buffer = (char*)malloc(HManager::TargetSize);
		memset(HManager::Buffer, 0x00, HManager::TargetSize);
		if (fread(HManager::Buffer, sizeof(char), HManager::TargetSize, HManager::TargetFile) != HManager::TargetSize)
		{
			cout << Color(AC_RED) << "Unable to read " << HManager::TargetSize << " Bytes because of insufficient memory...\n";
			_getch();
			exit(0);
		}
		fclose(HManager::TargetFile);
	}
	HManager::ReadHeader();
}

void HManager::Init(short int ActionType, char* TargetName, short int FilterType, char* FilterValue)
{
	HManager::Init(ActionType, TargetName);
	// TODO: The rest.
}
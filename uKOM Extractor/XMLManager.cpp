#include "XMLManager.h"
//#include <advconsole.h>
#include <conio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <rapidxml.hpp>
#include <rapidxml_utils.hpp>
#include "EManager.h"
#pragma warning (disable: 4996)
using std::cout; using std::endl;
using namespace std;
using namespace AdvancedConsole;
using namespace rapidxml;

char* XMLManager::XMLBuff = nullptr;
XMLManager::_XML_ENTRY XMLManager::Files;
vector<XMLManager::_XML_ENTRY> XMLManager::FilesList;
unsigned long XMLManager::Size = NULL;
unsigned long XMLManager::XMLSize = NULL;
char* XMLManager::KOMName = nullptr;

#define HEADER_SIZE 72

void XMLManager::Init(char* XMLBuffer, unsigned long XMLSize, unsigned long KOMSize, char* KOMName)
{
	if (!XMLBuffer)
	{
		cout << Color(AC_RED) << "No XML Buffer to read.\n";
		_getch();
		exit(0);
	}
	XMLManager::XMLBuff = (XMLBuffer + HEADER_SIZE);
	XMLManager::Size = KOMSize;
	XMLManager::XMLSize = XMLSize;
	XMLManager::KOMName = KOMName;

	FILE* XMLFile = fopen("KOMInfo.xml", "wb");
	if (!XMLFile)
	{
		cout << Color(AC_RED) << "Unable to write KOMInfo.xml.\n";
		_getch();
		exit(0);
	}
	fwrite(XMLManager::XMLBuff, sizeof(char), XMLSize, XMLFile);
	fclose(XMLFile);

	XMLManager::ReadXML();
}

void XMLManager::ReadXML()
{
	if (!XMLManager::XMLBuff)
	{
		cout << Color(AC_RED) << "No XML Buff to read.\n";
		_getch();
		exit(0);
	}

	xml_document<> doc;
	std::ifstream file("KOMInfo.xml");
	std::stringstream bufferx;
	bufferx << file.rdbuf();
	file.close();
	std::string content(bufferx.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node();
	for (xml_node<> *pNode = pRoot->first_node("File"); pNode; pNode = pNode->next_sibling())
	{
		XMLManager::Files.Algorithm = NULL;
		//HManager::Files.Checksum		= nullptr;
		XMLManager::Files.CompressedSize = NULL;
		//HManager::Files.FileTime		= { 0 };
		//HManager::Files.FName			= { 0 };
		XMLManager::Files.Size = NULL;
		// File name.
		xml_attribute<> *pAttr = pNode->first_attribute("Name");
		//HManager::Files.FName = pAttr->value();
		strcpy(XMLManager::Files.FName, pAttr->value() + '\0');
		// Its size.
		pAttr = pNode->first_attribute("Size");
		XMLManager::Files.Size = atoi(pAttr->value());
		// Its compressed size.
		pAttr = pNode->first_attribute("CompressedSize");
		XMLManager::Files.CompressedSize = atoi(pAttr->value());
		// Checksum.
		pAttr = pNode->first_attribute("Checksum");
		//HManager::Files.Checksum = pAttr->value();
		strcpy(XMLManager::Files.Checksum, pAttr->value() + '\0');
		// File timestamp (even if we don't care)
		pAttr = pNode->first_attribute("FileTime");
		//HManager::Files.FileTime = pAttr->value();
		strcpy(XMLManager::Files.FileTime, pAttr->value() + '\0');
		// MOST IMPORTANT: Algorithm type.
		pAttr = pNode->first_attribute("Algorithm");
		XMLManager::Files.Algorithm = atoi(pAttr->value());
		
		XMLManager::FilesList.push_back(Files);
	}
	//HManager::LastOffset = ((HManager::HBuffer + DEFAULT_HEADER_SIZE) + HManager::KOMHeader.XMLSize);
	EManager::Init(XMLManager::XMLBuff, XMLManager::Size, XMLManager::FilesList, XMLManager::XMLSize, XMLManager::KOMName);
}

inline bool XMLManager::CheckFile(char* FileName) 
{
	if (FILE *file = fopen(FileName, "rb")) {
		fclose(file);
		return true;
	}
	else {
		return false;
	}
}

unsigned long XMLManager::GetXML(char* XMLBuffer, unsigned long BuffersSize, unsigned long FNum)
{
	if (!CheckFile("KOMInfo.xml"))
	{
		cout << Color(AC_RED) << "KOMInfo.xml does not exists or is not accessible!\n";
		_getch();
		exit(0);
	}

	unsigned long BuffersSized = NULL;
	unsigned long FNumd = NULL;
	xml_document<> doc;
	std::ifstream file("KOMInfo.xml");
	std::stringstream bufferx;
	bufferx << file.rdbuf();
	file.close();
	std::string content(bufferx.str());
	doc.parse<0>(&content[0]);

	xml_node<> *pRoot = doc.first_node();
	for (xml_node<> *pNode = pRoot->first_node("File"); pNode; pNode = pNode->next_sibling())
	{
		XMLManager::Files.Algorithm = NULL;
		//HManager::Files.Checksum		= nullptr;
		XMLManager::Files.CompressedSize = NULL;
		//HManager::Files.FileTime		= { 0 };
		//HManager::Files.FName			= { 0 };
		XMLManager::Files.Size = NULL;
		// File name.
		xml_attribute<> *pAttr = pNode->first_attribute("Name");
		//HManager::Files.FName = pAttr->value();
		strcpy(XMLManager::Files.FName, pAttr->value() + '\0');
		// Its size.
		pAttr = pNode->first_attribute("Size");
		XMLManager::Files.Size = atoi(pAttr->value());
		// Its compressed size.
		pAttr = pNode->first_attribute("CompressedSize");
		XMLManager::Files.CompressedSize = atoi(pAttr->value());
		// Checksum.
		pAttr = pNode->first_attribute("Checksum");
		//HManager::Files.Checksum = pAttr->value();
		strcpy(XMLManager::Files.Checksum, pAttr->value() + '\0');
		// File timestamp (even if we don't care)
		pAttr = pNode->first_attribute("FileTime");
		//HManager::Files.FileTime = pAttr->value();
		strcpy(XMLManager::Files.FileTime, pAttr->value() + '\0');
		// MOST IMPORTANT: Algorithm type.
		pAttr = pNode->first_attribute("Algorithm");
		XMLManager::Files.Algorithm = atoi(pAttr->value());

		BuffersSized += XMLManager::Files.CompressedSize;
		FNumd++;

		XMLManager::FilesList.push_back(Files);
	}

	BuffersSize = BuffersSized;
	FNum = FNumd;
	XMLBuffer = (char*)content.c_str();
}
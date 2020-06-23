#define _CRT_SECURE_NO_WARNINGS
#define ERR_CREATE_FILEMAP 4
#define ERR_MAP_FILEVIEW   5
#define ERR_ACCESS_FILEVIEW 6

#include <Windows.h>

#include <iostream>

#include "SharedMemory.hpp"

using namespace std;

void SHM::launchEXE(std::string fileName) {
	STARTUPINFOA StartupInfo = { sizeof StartupInfo };
	PROCESS_INFORMATION ProcessInfo = { 0 };
	if (!CreateProcessA(
		NULL,   // name
		const_cast<char*>(fileName.c_str()), // Command line argument(s), const_cast is necessary because c_str() is a const char pointer
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&StartupInfo,            // Pointer to STARTUPINFO structure
		&ProcessInfo)           // Pointer to PROCESS_INFORMATION structure
		) {
		std::cerr << "Could not launch process: " << fileName << std::endl;
		throw std::exception("Could not launch process");
	}

	// Close process and thread handles. 
	CloseHandle(ProcessInfo.hProcess);
	CloseHandle(ProcessInfo.hThread);
}

struct SMObject::PIMPL {
	HANDLE CreateHandle = NULL;
	HANDLE AccessHandle = NULL;
	TCHAR* szname = NULL;
};

SMObject::SMObject(std::string szname, int size) : pimpl(new PIMPL) {
	pimpl->szname = new TCHAR[szname.size() + 1]; // ugly conversion from string to TCHAR
	pimpl->szname[szname.size()] = '\0';
	std::copy(szname.begin(), szname.end(), pimpl->szname);
	this->size = size;
}

SMObject::~SMObject() {
	if (pimpl->CreateHandle != NULL) CloseHandle(pimpl->CreateHandle);
	if (pimpl->AccessHandle != NULL) CloseHandle(pimpl->AccessHandle);
	if (pData != NULL) UnmapViewOfFile(pData);
	if (pimpl->szname != NULL) delete[] pimpl->szname;
}

bool SMObject::SMCreate() {
	pimpl->CreateHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, pimpl->szname); //read/write access
	return pimpl->CreateHandle == NULL ? false : true;
}

bool SMObject::SMAccess() {
	//Requesting a view of Modules memory
	pimpl->AccessHandle = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, pimpl->szname);
	if (pimpl->AccessHandle == NULL) 
		return false;

	//Requesting access to Modules memory
	pData = MapViewOfFile(pimpl->AccessHandle, FILE_MAP_ALL_ACCESS, 0, 0, size);
	if (pData == NULL) 
		return false;

	return true;
}
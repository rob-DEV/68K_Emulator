#include "memory_map_file.h"
#include <stdio.h>
#include <conio.h>
#include <tchar.h>

static MemoryMapFile* m_Instance = NULL;

MemoryMapFile::MemoryMapFile()
{

}


MemoryMapFile::~MemoryMapFile()
{
}

MemoryMapFile* MemoryMapFile::getInstance()
{
	if (m_Instance == NULL)
	m_Instance = new MemoryMapFile();

	return m_Instance;
}
void MemoryMapFile::map()
{

	m_MapFile = CreateFileMapping(
		INVALID_HANDLE_VALUE,    // use paging file
		NULL,                    // default security
		PAGE_READWRITE,          // read/write access
		0,                       // maximum object size (high-order DWORD)
		BUFFER_SIZE,                // maximum object size (low-order DWORD)
		MC68K_SHARED_MEMORY_OBJECT);                 // name of mapping object

	if (m_MapFile == NULL)
	{
		_tprintf(TEXT("Could not create file mapping object (%d).\n"),
			GetLastError());
		return;
	}
	m_Buffer = (LPTSTR)MapViewOfFile(m_MapFile,   // handle to map object
		FILE_MAP_ALL_ACCESS, // read/write permission
		0,
		0,
		BUFFER_SIZE);

	if (m_Buffer == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(m_MapFile);

		return;
	}

}

std::string MemoryMapFile::open()
{
	m_MapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		MC68K_SHARED_MEMORY_OBJECT);               // name of mapping object

	if (m_MapFile == NULL)
	{
		_tprintf(TEXT("Could not open file mapping object (%d).\n"),
			GetLastError());
		return std::string();
	}

	m_Buffer = (LPTSTR)MapViewOfFile(m_MapFile, // handle to map object
		FILE_MAP_ALL_ACCESS,  // read/write permission
		0,
		0,
		BUFFER_SIZE);

	if (m_Buffer == NULL)
	{
		_tprintf(TEXT("Could not map view of file (%d).\n"),
			GetLastError());

		CloseHandle(m_MapFile);

		return std::string();
	}
	
	std::string line(m_Buffer);
	UnmapViewOfFile(m_Buffer);
	CloseHandle(m_MapFile);
	return line;
}

void MemoryMapFile::pushStringToFile(const char* str)
{
	if (m_MapFile && m_Buffer)
	{
		CopyMemory((PVOID)m_Buffer, "AIDS", (_tcslen("AIDS") * sizeof(TCHAR)));
	}
}

void MemoryMapFile::close()
{
	UnmapViewOfFile(m_Buffer);
	CloseHandle(m_MapFile);
}


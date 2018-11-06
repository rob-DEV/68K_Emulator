#pragma once
#include <Windows.h>
#include <string>

#define MC68K_SHARED_MEMORY_OBJECT  "Global\\MC68K_POOL"
#define BUFFER_SIZE 256

#define MEMORY_MAPPED_FILE MemoryMapFile::getInstance()

class MemoryMapFile
{
private:
	HANDLE m_MapFile;
	LPCTSTR m_Buffer;

public:
	MemoryMapFile();
	~MemoryMapFile();

	static MemoryMapFile* getInstance();

	void map();
	std::string open();
	void pushStringToFile(const char* str);
	void close();

	
};


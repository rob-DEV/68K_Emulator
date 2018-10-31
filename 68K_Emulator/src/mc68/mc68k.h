#pragma once
#include "../common/structures.h"
#include <vector>
#include <string>

#define MEMORY_SIZE 16 * 1024 * 1024

class MC68K
{
private:

	std::vector<SourceLine> m_loadedProgramInstructions;
	void init();

public:
	MC68K();
	~MC68K();

	void load(std::vector<SourceLine> lexedProgram);
	void execute();
	int m_RegD0;
	int m_RegD1;
	int m_RegD2;
	int m_RegD3;
	int m_RegD4;
	int m_RegD5;
	int m_RegD6;
	int m_RegD7;

	int m_RegA0;
	int m_RegA1;
	int m_RegA2;
	int m_RegA3;
	int m_RegA4;
	int m_RegA5;
	int m_RegA6;
	int m_RegA7;

	char* m_Memory;
};


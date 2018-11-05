#pragma once
#include "../common/structures.h"
#include <console.h>
#include <vector>
#include <string>

#define MEMORY_SIZE 16 * 1024 * 1024

enum REG_OPER{ _ADD, _SUB, _SET };

class MC68K
{
private:

	std::vector<SourceLine> m_loadedProgramInstructions;
	void init();

	void updateRegister(const std::string& reg, REG_OPER oper, unsigned int value);
	void readFromMemory(unsigned int address, void* buffer, unsigned int size) const;
	void writeToMemory(unsigned int address, unsigned int size, const std::string& reg);

	unsigned int m_programCounter = 0;

public:
	MC68K();
	~MC68K();

	void load(const std::vector<SourceLine>& lexedProgram);
	int execute();

	void dumpRegisters();

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


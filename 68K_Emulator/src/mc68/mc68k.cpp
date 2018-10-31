#include "mc68k.h"
#include "instruction_set.h"

#include <console.h>
#include <cstring>


MC68K::MC68K()
{
	init();
}

void MC68K::init()
{
	//data registers
	m_RegD0 = 0;
	m_RegD1 = 0;
	m_RegD2 = 0;
	m_RegD3 = 0;
	m_RegD4 = 0;
	m_RegD5 = 0;
	m_RegD6 = 0;
	m_RegD7 = 0;

	//address registers
	m_RegA0 = 0;
	m_RegA1 = 0;
	m_RegA2 = 0;
	m_RegA3 = 0;
	m_RegA4 = 0;
	m_RegA5 = 0;
	m_RegA6 = 0;
	m_RegA7 = 0;

	//initalize RAM
	m_Memory = new char[MEMORY_SIZE];

	//set each byte to FF (255)
	std::memset(m_Memory, 255, MEMORY_SIZE);
}

MC68K::~MC68K()
{
}

void MC68K::load(std::vector<SourceLine> lexedProgram)
{
	//there is no syntax check atm
	if (m_loadedProgramInstructions.size() > 0)
	{
		//existing program terminate and reset
	}

	//dodgy reset atm
	m_loadedProgramInstructions = lexedProgram;


}

void MC68K::execute()
{
	//this is an interpreted approach eventually this process will be done at compile time and then the raw
	//opcodes will be passed to the mk68 emulator

	if (m_loadedProgramInstructions.size() < 1)
	{
		CONSOLE.writeLine("Error: No program has been loaded!");
	}
	//there is no syntax check atm
	//memory is not available as there is no memory map, hence data can only be stored and manipulated in the registers

	//we avoid the START and END START labels
	std::string delimiter = " ";
	for (size_t i = 1; i < m_loadedProgramInstructions.size() -1; i++)
	{
		std::string raw_instruction = m_loadedProgramInstructions[i].line;
		CONSOLE.writeLine(raw_instruction);

		//first get each instrucation type and then execute it appropriately
		
		std::string operation = raw_instruction.substr(0, raw_instruction.find(" "));

		//instruction parsing
		//the parsing is very strict
		if (operation == MOVE_W) {

			short dataToMove = 0;

			//pattern MOVE.W src/data, DEST
			//pattern MOVE.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1)- raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);
			std::string::size_type sz;
			
			if (oper1.find("#") != std::string::npos)
			{
				//literal value
				std::string oper1ValStr = oper1.substr(1);
				dataToMove = std::stoi(oper1ValStr, &sz);
			}

			if (oper2 == "D0")
				m_RegD0 = dataToMove;
			if (oper2 == "D1")
				m_RegD1 = dataToMove;
			if (oper2 == "D2")
				m_RegD2 = dataToMove;
			if (oper2 == "D3")
				m_RegD3 = dataToMove;
			if (oper2 == "D4")
				m_RegD4 = dataToMove;
			if (oper2 == "D5")
				m_RegD5 = dataToMove;
			if (oper2 == "D6")
				m_RegD6 = dataToMove;
			if (oper2 == "D7")
				m_RegD7 = dataToMove;
		}

	}


}
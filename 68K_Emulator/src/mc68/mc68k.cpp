#include "mc68k.h"
#include "constants.h"
#include <console.h>
#include <algorithm>
#include <thread>

MC68K::MC68K()
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

	m_OutputTerminal = new MemoryMapFile();
	m_OutputTerminal->map();
}

MC68K::~MC68K()
{
	delete[] m_Memory;
	delete m_OutputTerminal;
}

void MC68K::load(const std::vector<SourceLine>& lexedProgram)
{
	//there is no syntax check atm
	if (m_loadedProgramInstructions.size() > 0)
	{
		//existing program terminate and reset
		CONSOLE.writeLine("ERROR: No program has been loaded!");
		return;
	}

	//dodgy reset atm
	m_loadedProgramInstructions = lexedProgram;


}

int parseLiteralValue(const std::string& literalOperand)
{
	int result = 0;
	std::string::size_type sz;
	std::string oper1ValStr = literalOperand.substr(1);
	result = std::stoi(oper1ValStr, &sz);
	return result;

}

std::string parseCommandGetOperation(const std::string& raw_text_instruction)
{
	std::string operation = raw_text_instruction.substr(0, raw_text_instruction.find(" "));
	return operation;
}

void MC68K::updateRegister(const std::string& reg, REG_OPER oper, int value)
{
	if (oper == REG_OPER::_SET) {
		if (reg == D0)
			m_RegD0 = value;
		else if (reg == D1)
			m_RegD1 = value;
		else if (reg == D2)
			m_RegD2 = value;
		else if (reg == D3)
			m_RegD3 = value;
		else if (reg == D4)
			m_RegD4 = value;
		else if (reg == D5)
			m_RegD5 = value;
		else if (reg == D6)
			m_RegD6 = value;
		else if (reg == D7)
			m_RegD7 = value;

		//set CCR
		if (value < 0)
			m_N = 1;
		else
			m_N = 0;
	}
	else if (oper == REG_OPER::_ADD) {
		if (reg == D0)
			m_RegD0 += value;
		else if (reg == D1)
			m_RegD1 += value;
		else if (reg == D2)
			m_RegD2 += value;
		else if (reg == D3)
			m_RegD3 += value;
		else if (reg == D4)
			m_RegD4 += value;
		else if (reg == D5)
			m_RegD5 += value;
		else if (reg == D6)
			m_RegD6 += value;
		else if (reg == D7)
			m_RegD7 += value;
	}
}

void MC68K::readFromMemory(unsigned int address, void* buffer, unsigned int size) const
{
	memcpy(buffer, &m_Memory[address], size);
}

void MC68K::writeToMemory(unsigned int address, unsigned int size, const std::string& reg)
{
	if (address > MEMORY_SIZE - 1)
		return;

	int* regWithVal = NULL;

	if (reg == D0)
		regWithVal = &m_RegD0;
	else if (reg == D1)
		regWithVal = &m_RegD1;
	else if (reg == D2)
		regWithVal = &m_RegD2;
	else if (reg == D3)
		regWithVal = &m_RegD3;
	else if (reg == D4)
		regWithVal = &m_RegD4;
	else if (reg == D5)
		regWithVal = &m_RegD5;
	else if (reg == D6)
		regWithVal = &m_RegD6;
	else if (reg == D7)
		regWithVal = &m_RegD7;

	void* baseAddress = &m_Memory[address];

	memcpy(baseAddress, regWithVal, size);
}

int MC68K::execute()
{
	//this is an interpreted approach eventually this process will be done at compile time and then the raw
	//opcodes will be passed to the mk68 emulator

	if (m_loadedProgramInstructions.size() < 1) 
	{
		CONSOLE.writeLine("Error: No program has been loaded!");
		return -1;
	}
	//there is no syntax check atm
	//memory is not available as there is no memory map, hence data can only be stored and manipulated in the registers

	//we avoid the START and END START labels
	while(true)
	{
		//end of execution
		if (m_programCounter > m_loadedProgramInstructions.size())
			break;

		//limit rate of execution for testing
		std::this_thread::sleep_for(std::chrono::milliseconds(300));

		std::string raw_instruction = m_loadedProgramInstructions[m_programCounter].line;
		
		//output to terminal test
		m_OutputTerminal->pushStringToFile(std::string(raw_instruction + "\n").c_str());


		//first get each instrucation type and then execute it appropriately
		
		std::string operation = parseCommandGetOperation(raw_instruction);
		std::string operation_size;
		try {
			operation_size = operation.substr(raw_instruction.find(".")+1);
		}
		catch(const std::exception& ex)
		{};
		//instruction parsing
		//the parsing is very strict
		if (operation.find(MOVE) != std::string::npos) {
			int data = 0;

			if (operation_size == BYTE)
				data = (char)data;
			if (operation_size == WORD)
				data = (short)data;
			if (operation_size == LONG)
				data = (int)data;
			

			//pattern MOVE.W src/data, DEST
			//pattern MOVE.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos) {
				//not hex
				if (oper1.find("$") == std::string::npos)
					data = (short)parseLiteralValue(oper1);
			}
			else if (oper1.find("$") != std::string::npos) {
				//not a literal value - instead read from a memory location
				//convert the hex str to an int
				std::string hexValStr = oper1.substr(oper1.find('$') + 1);
				unsigned int x = std::stoul(hexValStr, nullptr, 16);

				readFromMemory(x, &data, 2);
			}

			if (oper2.find("$") != std::string::npos) {
				//not a literal value - instead read from a memory location
				//convert the hex str to an int
				std::string hexValStr = oper2.substr(oper1.find('$') + 2);
				unsigned int x = std::stoul(hexValStr, nullptr, 16);

				writeToMemory(x, sizeof(short), oper1);
			}
			else {
				//register assumed to be default THIS IS A HORRIBLE IDEA
				updateRegister(oper2, REG_OPER::_SET, data);
			}
		}
		//NOTE THIS CAN HANDLE ADD AND SUB AS SUB IS JUST MINUS ADDITION
		else if (operation.find(ADD) != std::string::npos || operation.find(SUB) != std::string::npos) {

			char data = 0;

			//pattern ADD.W data, DEST
			//pattern ADD.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos)
				data = (char)parseLiteralValue(oper1);


			//check if operation is SUB
			//if so change oper 2 to negative
			if (operation.find(SUB) != std::string::npos)
				data = -data;

			if (operation_size == BYTE)
				data = (char)data;
			if (operation_size == WORD)
				data = (short)data;
			if (operation_size == LONG)
				data = (int)data;

			updateRegister(oper2, REG_OPER::_ADD, data);
		}
		else if (operation.find(BRA) != std::string::npos) {
			//get label find it in the program code then JMP to that line
			std::string branchLabel = raw_instruction.substr(raw_instruction.find(' ') + 1);

			//find where the label exists in the code and set the program counter to that instruction
			bool labelFound = false;
			for (size_t i = 0; i < m_loadedProgramInstructions.size(); i++)
			{
				if ((m_loadedProgramInstructions[i].line.find("BRA") == std::string::npos) && m_loadedProgramInstructions[i].line.find(branchLabel) != std::string::npos)
				{
					m_programCounter = m_loadedProgramInstructions[i].lineNumber;
					labelFound = true;
					break;
				}
			}
			//matching BRA label not found exit -1
			if (!labelFound) 
			{
				CONSOLE.writeLine("ERROR: Label %s not found exiting\n", branchLabel.c_str());
				return -1;
			}
		}
		else if (operation == TRAP) {
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1);
			int data = parseLiteralValue(oper1);

			//exit routine
			if (m_RegD0 == 9 && data == 15)
				return 0;
		}
		else if (operation == SIMHALT) {
			return 0;
		}

		//increment the program counter
		++m_programCounter;
	}
}

void MC68K::dumpRegisters()
{
	
	CONSOLE.write("REGISTER DUMP\n-----------------------------------\nD0 = %d\nD1 = %d\nD2 = %d\nD3 = %d\nD4 = %d\nD5 = %d\nD6 = %d\nD7 = %d", m_RegD0, m_RegD1, m_RegD2, m_RegD3, m_RegD4, m_RegD5, m_RegD6, m_RegD7);
	CONSOLE.write("\n-----------------------------------\nA0 = %d\nA1 = %d\nA2 = %d\nA3 = %d\nA4 = %d\nA5 = %d\nA6 = %d\nA7 = %d\n-----------------------------------\nREGISTER DUMP END\n", m_RegA0, m_RegA1, m_RegA2, m_RegA3, m_RegA4, m_RegA5, m_RegA6, m_RegA7);
}
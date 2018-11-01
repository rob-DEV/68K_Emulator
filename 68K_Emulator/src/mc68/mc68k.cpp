#include "mc68k.h"
#include "constants.h"

#include <console.h>
#include <cstring>
#include <algorithm>


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

void MC68K::updateRegister(const std::string& reg, REG_OPER oper, unsigned int value)
{
	if (oper == REG_OPER::SET) {
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
	}
	else if (oper == REG_OPER::ADD) {
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

void MC68K::readFromMemory(unsigned int address, void* buffer, unsigned int size)
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

void MC68K::execute()
{
	//this is an interpreted approach eventually this process will be done at compile time and then the raw
	//opcodes will be passed to the mk68 emulator

	if (m_loadedProgramInstructions.size() < 1) {
		CONSOLE.writeLine("Error: No program has been loaded!");
		return;
	}
	//there is no syntax check atm
	//memory is not available as there is no memory map, hence data can only be stored and manipulated in the registers

	//we avoid the START and END START labels
	for (size_t i = 1; i < m_loadedProgramInstructions.size() -1; i++)
	{
		std::string raw_instruction = m_loadedProgramInstructions[i].line;
		//CONSOLE.writeLine(raw_instruction);

		//first get each instrucation type and then execute it appropriately
		
		std::string operation = parseCommandGetOperation(raw_instruction);

		//instruction parsing
		//the parsing is very strict
		if (operation == MOVE_B) {

			char data = 0;

			//pattern MOVE.W src/data, DEST
			//pattern MOVE.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);

			if (oper1.find("#") != std::string::npos)
				data = (char)parseLiteralValue(oper1);

			updateRegister(oper2, REG_OPER::SET, data);
		} 
		else if (operation == MOVE_W) {

			short data = 0;

			//pattern MOVE.W src/data, DEST
			//pattern MOVE.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1)- raw_instruction.find(" "));

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
				updateRegister(oper2, REG_OPER::SET, data);
			}

			
		}
		else if (operation == MOVE_L) {

			int data = 0;

			//pattern MOVE.W src/data, DEST
			//pattern MOVE.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos)
				data = (int)parseLiteralValue(oper1);


			updateRegister(oper2, REG_OPER::SET, data);
		}
		//NOTE THIS CAN HANDLE ADD AND SUB AS SUB IS JUST MINUS ADDITION
		else if (operation == ADD_B || operation == SUB_B) {

			char data = 0;

			//pattern ADD.W data, DEST
			//pattern ADD.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos)
				data = (char)parseLiteralValue(oper1);


			//check if operation is SUB
			//if so change oper 2 to negative
			if (operation == SUB_B)
				data = -data;

			updateRegister(oper2, REG_OPER::ADD, data);
		}
		else if (operation == ADD_W || operation == SUB_W) {

			short data = 0;

			//pattern ADD.W data, DEST
			//pattern ADD.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos)
				data = (short)parseLiteralValue(oper1);

			if (operation == SUB_W)
				data = -data;

			updateRegister(oper2, REG_OPER::ADD, data);
		}
		else if (operation == ADD_L || operation == SUB_L) {

			int data = 0;

			//pattern ADD.W data, DEST
			//pattern ADD.W oper1, oper2
			std::string oper1 = raw_instruction.substr(raw_instruction.find(' ') + 1, (raw_instruction.find(',') - 1) - raw_instruction.find(" "));

			std::string oper2 = raw_instruction.substr(raw_instruction.find(',') + 2);


			if (oper1.find("#") != std::string::npos)
				data = (int)parseLiteralValue(oper1);

			if (operation == SUB_L)
				data = -data;

			updateRegister(oper2, REG_OPER::ADD, data);
		}
	}


}

void MC68K::dumpRegisters()
{
	
	CONSOLE.write("REGISTER DUMP\n-----------------------------------\nD0 = %d\nD1 = %d\nD2 = %d\nD3 = %d\nD4 = %d\nD5 = %d\nD6 = %d\nD7 = %d", m_RegD0, m_RegD1, m_RegD2, m_RegD3, m_RegD4, m_RegD5, m_RegD6, m_RegD7);
	CONSOLE.write("\n-----------------------------------\nA0 = %d\nA1 = %d\nA2 = %d\nA3 = %d\nA4 = %d\nA5 = %d\nA6 = %d\nA7 = %d\n-----------------------------------\nREGISTER DUMP END\n", m_RegA0, m_RegA1, m_RegA2, m_RegA3, m_RegA4, m_RegA5, m_RegA6, m_RegA7);
}



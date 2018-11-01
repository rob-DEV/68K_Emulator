#include <ctime>
#include <console.h>
#include <object.h>


#include "io/io.h"
#include "mc68/mc68k.h"
#include "lexer/lexer.h"
#include "compiler/compiler.h"


int main(int argc, const char** argv)
{
	std::string inputASM = IO::readFile("res/src.68asm");
	
	//interpret the source file
	Lexer* lexer = new Lexer(inputASM);

	//send the program instructions and data to the compiler
	//Compiler* compiler = new Compiler();

	MC68K* mc68K = new MC68K();
	//eventually load the compiled program into the emulator
	mc68K->load(lexer->m_ProgramSourceLines);
	
	clock_t begin = clock();

	mc68K->execute();

	clock_t end = clock();
	double elapsed_milli_secs = double(end - begin) / CLOCKS_PER_SEC * 1000;

	mc68K->dumpRegisters();

	CONSOLE.writeLine("exe time: %.5f", elapsed_milli_secs);

	CONSOLE.readLine();
}
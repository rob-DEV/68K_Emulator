#include <console.h>
#include <object.h>
#include <chrono>

#include "io/io.h"
#include "mc68/mc68k.h"
#include "lexer/lexer.h"

using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;

int main(int argc, const char** argv)
{
	
	std::string inputASM = IO::readFile("res/src.68asm");
	
	//interpret the source file
	Lexer* lexer = new Lexer(inputASM);

	MC68K* mc68K = new MC68K();
	//eventually load the compiled program into the emulator
	mc68K->load(lexer->m_ProgramSourceLines);
	
	time_point<Clock> start = Clock::now();
	
	int exitCode = mc68K->execute();

	time_point<Clock> end = Clock::now();
	milliseconds diff = duration_cast<milliseconds>(end - start);

	mc68K->dumpRegisters();

	CONSOLE.writeLine("Execution time: %dms", diff.count());
	CONSOLE.writeLine("Exit Code: %d", exitCode);

	CONSOLE.readLine();
}
#include <ctime>
#include <console.h>
#include <object.h>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>

#include "io/io.h"
#include "io/memory_map_file.h"
#include "mc68/mc68k.h"
#include "lexer/lexer.h"

using Clock = std::chrono::steady_clock;
using std::chrono::time_point;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using namespace std::literals::chrono_literals;
using std::this_thread::sleep_for;

int main(int argc, const char** argv)
{
	
	MemoryMapFile s;
	s.map();
	
	while (true)
	{
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);
		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
		std::string a(oss.str());

		s.pushStringToFile(a.c_str());
	}
	
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
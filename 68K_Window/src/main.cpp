#include <console.h>
#include "io/memory_map_file.h"

int main()
{
	//access std::vector<std::string> shared memory

	//poll the shared memory for new strings and print them to the console
	MemoryMapFile s;
	const char* strStore = "";
	while (true)
	{
		s.open();
	}

	CONSOLE.readLine();
}
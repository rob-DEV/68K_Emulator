#include <chrono>
#include <thread>
#include <console.h>
#include "io/memory_map_file.h"

int main()
{
	//accessshared memory
	//poll the shared memory for new strings and print them to the console
	
	//duplicates are not allowed atm, i.e. "ABC" could not be printed twice in a row
	MemoryMapFile s;
	std::string store;
	while (true)
	{
		std::string a = s.open();
		if (a.length() > 0 && store != a) {
			printf("%s", a.c_str());
			//store the previous string to avoid duplication
			store = a;
		}
		//limit rate of execution for testing
		std::this_thread::sleep_for(std::chrono::milliseconds(300));
	}

	CONSOLE.readLine();
}
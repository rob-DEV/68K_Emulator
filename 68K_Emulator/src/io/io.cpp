#include "io.h"

std::string IO::readFile(const char* filepath)
{
	FILE* file = fopen(filepath, "rt");

	if (!file)
		return std::string();
	fseek(file, 0, SEEK_END);
	unsigned long length = ftell(file);
	//add 1 for termination char
	char* data = new char[length + 1];
	memset(data, 0, length + 1);
	//go back to the start
	fseek(file, 0, SEEK_SET);
	fread(data, 1, length, file);
	fclose(file);

	std::string result(data);
	delete[] data;
	return result;
}
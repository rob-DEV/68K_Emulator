#pragma once
#include "../common/structures.h"
#include <string>
#include <vector>




class Compiler
{

public:
	Compiler(std::vector<SourceLine> lexedProgram, std::vector<SourceLine> lexedData);
	~Compiler();
};


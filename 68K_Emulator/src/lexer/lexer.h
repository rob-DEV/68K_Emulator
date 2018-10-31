#pragma once
#include "../common/structures.h"
#include <string>
#include <vector>




class Lexer
{
public:
	std::vector<SourceLine> m_AllSourceLines;
	std::vector<SourceLine> m_ProgramSourceLines;
	std::vector<SourceLine> m_DataSourceLines;

public:
	Lexer(std::string fileSrc);
	~Lexer();

	inline const std::vector<SourceLine>& getSourceLines() { return m_AllSourceLines; }
};


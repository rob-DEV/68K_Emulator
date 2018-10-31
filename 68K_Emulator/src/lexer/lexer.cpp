#include "lexer.h"
#include <console.h>

Lexer::Lexer(std::string fileSrc) {

	std::string::size_type pos = 0;
	std::string::size_type prev = 0;
	uint32_t count = 1;
	while ((pos = fileSrc.find('\n', prev)) != std::string::npos)
	{
		SourceLine tmpSrcLine;
		tmpSrcLine.lineNumber = count;
		tmpSrcLine.line = fileSrc.substr(prev, pos - prev);
		
		if (tmpSrcLine.line != "")
			m_AllSourceLines.push_back(tmpSrcLine);

		prev = pos + 1;
		count++;
	}

	// To get the last substring (or only, if delimiter is not found)
	if (m_AllSourceLines.size() < 1) {
		SourceLine tmpSrcLine;
		tmpSrcLine.lineNumber = 1;
		tmpSrcLine.line = fileSrc;

		m_AllSourceLines.push_back(tmpSrcLine);
	}
	//take the source lines and section off the program instructions between start and end
	std::vector<SourceLine> programSourceLines;
	std::vector<SourceLine> dataSourceLines;

	//START: - entry point 
	int startIndex = -1;
	for (size_t i = 0; i < m_AllSourceLines.size(); i++)
		if (m_AllSourceLines[i].line.find("START:") != std::string::npos)
			startIndex = i;

	if (startIndex == -1)
	{
		CONSOLE.writeLine("Error: No entry point found!! exiting lexer!");
		return;
	}

	//ORG
	int orgIndex = -1;
	for (size_t i = 0; i < m_AllSourceLines.size(); i++)
		if (m_AllSourceLines[i].line.find("ORG") != std::string::npos)
			orgIndex = i;

	//NO ERROR doesnot matter atm

	//END START: - entry point 
	int endStartIndex = -1;
	for (size_t i = 0; i < m_AllSourceLines.size(); i++)
		if (m_AllSourceLines[i].line.find("END    START") != std::string::npos)
			endStartIndex = i;

	if (endStartIndex == -1)
	{
		CONSOLE.writeLine("Error: No end point found!! exiting lexer!");
		return;
	}

	//simple instructions test
	//pull all lines from start to start end assuming that they are sequencial
	for (size_t i = startIndex; i <= endStartIndex; i++)
		programSourceLines.push_back(m_AllSourceLines[i]);


	m_ProgramSourceLines = programSourceLines;
}

Lexer::~Lexer() {

}
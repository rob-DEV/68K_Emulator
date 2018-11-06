#include "console.h"

Console* m_pThis = NULL;

Console& Console::getConsole()
{
	if (m_pThis == NULL)
		m_pThis = new Console();
	static std::unique_ptr<Console, m_ConsoleDeleter> instance(m_pThis);
	return *instance;
}

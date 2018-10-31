#ifndef _CONSOLE_H_
#define _CONSOLE_H_
#include <iostream>
#include <memory>
#include <string>
#include <cstdarg>
#include <typeinfo>

class Console
{
private:

	Console() {};
	Console(const Console&) {};
	Console& operator=(const Console&) { return *this; };

	struct m_ConsoleDeleter
	{
		inline void operator()(const Console* const p)
		{
			delete p;
		}
	};

	template <class T, class = void>
	struct HasInserter :
		std::false_type
	{};

	template <class T>
	struct HasInserter<T, std::void_t<decltype(std::declval<std::ostream&>() << std::declval<const T>())>> :
		std::true_type
	{};

	template <class T>
	void writeLine(const T &t, std::true_type)
	{
		std::cout << t << '\n';
	}

	template <class T>
	void writeLine(const T &t, std::false_type)
	{
		std::cout << typeid(T).name() << '\n';
	};


public:
	inline void writeLine()
	{
		std::cout << "\r\n";
	}
	template<typename T>
	inline void writeLine(const T& t)
	{
		writeLine(t, HasInserter<T>{});
	}


	inline void writeLine(const char* format, ...)
	{
		char* sMessage = NULL;
		int nLength = 0;
		va_list args;
		va_start(args, format);
		//  Return the number of characters in the string referenced the list of arguments.
		// _vscprintf doesn't count terminating '\0' (that's why +1)
		nLength = _vscprintf(format, args) + 1;
		sMessage = new char[nLength];
		vsprintf_s(sMessage, nLength, format, args);
		//vsprintf(sMessage, format, args);
		std::cout << sMessage << "\n";
		va_end(args);

		delete[] sMessage;
	}

	inline std::string readLine()
	{
		std::string str;
		std::getline(std::cin, str);
		return str;
	}
	
	static Console& getConsole();
};

extern Console* m_pThis;

#define CONSOLE Console::getConsole()


#endif // _CONSOLE_H


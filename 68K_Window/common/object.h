#ifndef _OBJECT_H
#define _OBJECT_H

#include <iostream>

class object {
public:
	object()
	{

	}

	virtual ~object()
	{

	}

	virtual std::string toString() const
	{
		return std::string("global base object");
	}

	inline friend std::ostream& operator<<(std::ostream& stream, const object& object)
	{
		stream << object.toString();
		return stream;
	}
};
#endif // _OBECT_H
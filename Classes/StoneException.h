#ifndef __Stone_StoneException_H__
#define __Stone_StoneException_H__

#include <string>
#include <exception>

#include "StoneMarcos.h"
#include "ASTree.h"

NS_STONE_BEGIN

class StoneException : public std::exception
{
public:
	StoneException(const std::string& m)
		:_msg(m)
	{
	}
	StoneException(const std::string& m, ASTree* t)
	{
		_msg = m + " " + t->getLocation();
	}

	virtual const char* what() const
	{
		return _msg.c_str();
	}
private:
	std::string _msg;
};
NS_STONE_END
#endif

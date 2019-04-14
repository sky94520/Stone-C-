#ifndef __Stone_Environment_H__
#define __Stone_Environment_H__

#include <string>

#include "StObject.h"
#include "Value.h"

NS_STONE_BEGIN

class Environment: public Object
{
public:
	virtual ~Environment() {}
	virtual void putNew(const std::string& name, const Value& value) = 0;
	virtual void put(const std::string& name, const Value& value) = 0;
	virtual const Value* get(const std::string& name) const = 0;
};
NS_STONE_END
#endif
#ifndef __Stone_ArrayParser_H__
#define __Stone_ArrayParser_H__

#include "ClosureParser.h"

NS_STONE_BEGIN

class Parser;

class ArrayParser : public ClosureParser
{
public:
	ArrayParser();
	virtual ~ArrayParser();
private:
	bool init();
protected:
	Parser* _elements;
};
NS_STONE_END
#endif
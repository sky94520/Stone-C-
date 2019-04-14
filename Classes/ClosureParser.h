#ifndef __Stone_ClosureParser_H__
#define __Stone_ClosureParser_H__

#include <vector>

#include "FuncParser.h"

NS_STONE_BEGIN

class ClosureParser : public FuncParser
{
public:
	//primary: ("(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix} | "fun" param_list block
	virtual ASTree* primary();
};
NS_STONE_END
#endif
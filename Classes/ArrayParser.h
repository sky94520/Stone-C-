#ifndef __Stone_ArrayParser_H__
#define __Stone_ArrayParser_H__

#include "ClosureParser.h"

NS_STONE_BEGIN
class ArrayParser : public ClosureParser
{
public:
	//ÊÇ·ñ»¹ÓÐpostfix
	virtual bool isHasPostfix();
	//postfix: "(" [args] ")" | "[" expr "]"
	virtual ASTree* postfix();

	//primary: ( "[" [elements] "]" | "(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix} | "fun" param_list block
	virtual ASTree* primary();

	//elements: expr { "," expr }
	ASTree* elements();
};
NS_STONE_END

#endif
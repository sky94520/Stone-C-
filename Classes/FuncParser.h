#ifndef __Stone_FuncParser_H__
#define __Stone_FuncParser_H__

#include <vector>

#include "BasicParser.h"

NS_STONE_BEGIN

class FuncParser : public BasicParser
{
public:
	//program: [def | statement] (";" | EOL)
	virtual ASTree* program();

	//simple: expr [args]
	virtual ASTree* simple();

	//primary: ("(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix}
	virtual ASTree* primary();

	//"(" [args] ")"
	ASTree* postfix();

	//args: expr {"," expr}
	ASTree* args();

	//def: "def" IDENTIFIER param_list block
	ASTree* def();

	//param_list: "(" [params] ")"
	ASTree* paramList();

	//params: param {"," param}
	ASTree* params();

	//param: IDENTIFIER
	ASTree* param();
};
NS_STONE_END
#endif
#include "ClosureParser.h"
#include "ClosureStmnt.h"
#include "ParameterList.h"
#include "BlockStmnt.h"

NS_STONE_BEGIN

ASTree* ClosureParser::primary()
{
	//primary: ("(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix} | "closure" param_list block
	if (isToken("closure"))
	{
		token("closure");
		std::vector<ASTree*> list;
		//形参列表
		list.push_back(this->paramList());
		//代码块
		list.push_back(this->block());

		return new ClosureStmnt(list);
	}
	else
		return FuncParser::primary();
}
NS_STONE_END
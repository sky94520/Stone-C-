#include "ArrayParser.h"
#include "ArrayLiteral.h"
#include "ArrayRef.h"

NS_STONE_BEGIN

bool ArrayParser::isHasPostfix()
{
	return isToken("[") || FuncParser::isHasPostfix();
}

ASTree* ArrayParser::postfix()
{
	//postfix: "(" [args] ")" | "[" expr "]"
	if (isToken("["))
	{
		token("[");
		auto expr = expression();
		token("]");
		return new ArrayRef(expr);
	}
	else
		return FuncParser::postfix();
}

ASTree* ArrayParser::primary()
{
	//primary: ( "{" [elements] "}" | "(" expression ")" | NUMBER | IDENTIFIER | STRING){postfix} | "fun" param_list block
	ASTree* elements = nullptr;
	if (isToken("{"))
	{
		token("{");
		if (!isToken("}"))
		{
			elements = this->elements();
		}
		token("}");

		if (elements == nullptr)
			elements = new ArrayLiteral();
		return elements;
	}
	else
		return ClosureParser::primary();
}

ASTree* ArrayParser::elements()
{
	//elements: expr { "," expr }
	std::vector<ASTree*> list;
	list.push_back(BasicParser::expression());

	while (isToken(","))
	{
		token(",");
		list.push_back(BasicParser::expression());
	}

	return new ArrayLiteral(list);
}
NS_STONE_END

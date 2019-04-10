#include "NumberLiteral.h"
#include "Token.h"
NS_STONE_BEGIN

NumberLiteral::NumberLiteral(Token* token)
	:ASTLeaf(token)
{
}

int NumberLiteral::getValue() const
{
	return getToken()->asInt();
}

NS_STONE_END
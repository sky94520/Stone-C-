#include "StringLiteral.h"
#include "Token.h"

NS_STONE_BEGIN

StringLiteral::StringLiteral(Token* token)
	:ASTLeaf(token)
{
}

std::string StringLiteral::getValue() const
{
	return getToken()->getText();
}

NS_STONE_END
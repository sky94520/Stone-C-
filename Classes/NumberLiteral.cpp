#include "NumberLiteral.h"
#include "Token.h"
#include "Visitor.h"

NS_STONE_BEGIN

NumberLiteral::NumberLiteral(Token* token)
	:ASTLeaf(token)
{
}

int NumberLiteral::getValue() const
{
	return getToken()->asInt();
}

void NumberLiteral::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}

NS_STONE_END
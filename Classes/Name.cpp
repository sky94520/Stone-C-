#include "Name.h"
#include "Token.h"
#include "Visitor.h"

NS_STONE_BEGIN

Name::Name(Token* token)
	:ASTLeaf(token)
{
}

std::string Name::getName() const
{
	return getToken()->asString();
}

void Name::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
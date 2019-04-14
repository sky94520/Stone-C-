#include "Postfix.h"
#include "Visitor.h"

NS_STONE_BEGIN
Postfix::Postfix()
{
}

Postfix::Postfix(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

Postfix::~Postfix()
{
}

void Postfix::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
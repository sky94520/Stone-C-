#include "PrimaryExpr.h"
#include "Visitor.h"

NS_STONE_BEGIN
PrimaryExpr::PrimaryExpr(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

PrimaryExpr::~PrimaryExpr()
{
}

void PrimaryExpr::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
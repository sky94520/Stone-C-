#include "NegativeExpr.h"
#include "ASTree.h"
#include "Visitor.h"

NS_STONE_BEGIN

NegativeExpr::NegativeExpr(ASTree* a)
{
	_children.push_back(a);
}

NegativeExpr::NegativeExpr(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

ASTree* NegativeExpr::getOperand()
{
	return getChild(0);
}

std::string NegativeExpr::toString() const
{
	return "-" + getChild(0)->toString();
}

void NegativeExpr::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
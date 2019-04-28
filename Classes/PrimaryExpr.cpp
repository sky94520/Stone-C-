#include "PrimaryExpr.h"
#include "Visitor.h"

NS_STONE_BEGIN
const std::string PrimaryExpr::TREE_ID = "PrimaryExpr";

PrimaryExpr::PrimaryExpr(const std::vector<ASTree*>& list)
	:ASTList(list)
{
	ASTree::init(TREE_ID);
}

PrimaryExpr::~PrimaryExpr()
{
}

void PrimaryExpr::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
#include "WhileStmnt.h"
#include "Visitor.h"

NS_STONE_BEGIN

WhileStmnt::WhileStmnt(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

ASTree* WhileStmnt::getCondition() const
{
	return getChild(0);
}

ASTree* WhileStmnt::getBody() const
{
	return getChild(1);
}

void WhileStmnt::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}

std::string WhileStmnt::toString() const
{
	return "(while " + getCondition()->toString() + " " + getBody()->toString() + ")";
}
NS_STONE_END
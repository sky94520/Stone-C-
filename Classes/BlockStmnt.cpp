#include "BlockStmnt.h"
#include "ASTree.h"
#include "Visitor.h"

NS_STONE_BEGIN

BlockStmnt::BlockStmnt(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

void BlockStmnt::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}
NS_STONE_END
#include "ASTree.h"
#include "Visitor.h"
#include "Environment.h"

NS_STONE_BEGIN

void ASTree::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}

NS_STONE_END
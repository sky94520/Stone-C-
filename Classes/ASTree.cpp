#include "ASTree.h"
#include "Visitor.h"
#include "Environment.h"

NS_STONE_BEGIN

const std::string ASTree::TREE_ID = "ASTree";

void ASTree::accept(Visitor* v, Environment* env)
{
	v->visit(this, env);
}

NS_STONE_END
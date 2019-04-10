#include "BinaryExpr.h"
#include "ASTLeaf.h"
#include "Token.h"

NS_STONE_BEGIN

BinaryExpr::BinaryExpr(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

BinaryExpr::BinaryExpr(ASTree* left, ASTLeaf* op, ASTree* right)
{
	_children.push_back(left);
	_children.push_back(op);
	_children.push_back(right);
}


BinaryExpr::~BinaryExpr()
{
}

ASTree* BinaryExpr::getLeft() const
{
	return getChild(0);
}

std::string BinaryExpr::getOperator() const
{
	return static_cast<ASTLeaf*>(getChild(1))->getToken()->asString();
}

ASTree* BinaryExpr::getRight() const
{
	return getChild(2);
}
NS_STONE_END

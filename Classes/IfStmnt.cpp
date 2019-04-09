#include "IfStmnt.h"
NS_STONE_BEGIN

IfStmnt::IfStmnt(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}

ASTree* IfStmnt::getCondition() const
{
	return getChild(0);
}

ASTree* IfStmnt::getThenBlock() const
{
	return getChild(1);
}

ASTree* IfStmnt::getElseBlock() const
{
	return getNumChildren() > 2 ? getChild(2) : nullptr;
}

std::string IfStmnt::toString() const
{
	std::stringstream in;
	in << "(if ";
	in << getCondition()->toString();
	in << " ";
	in << getThenBlock()->toString();
	if (getElseBlock() != nullptr)
		in << " else" << getElseBlock()->toString();
	in << ")";

	return in.str();
}

NS_STONE_END

#include "IfStmnt.h"
NS_STONE_BEGIN

IfStmnt::IfStmnt(const std::vector<ASTree*>& list, ASTree* elseBlock)
	:ASTList(list)
	,_elseBlock(elseBlock)
{
}

ASTree* IfStmnt::getCondition(unsigned int i) const
{
	return getChild(i * 2);
}

ASTree* IfStmnt::getThenBlock(unsigned int i) const
{
	return getChild(i * 2 + 1);
}

ASTree* IfStmnt::getElseBlock() const
{
	return _elseBlock;
}

unsigned int IfStmnt::getIfNumber() const
{
	return getNumChildren() / 2;
}

std::string IfStmnt::toString() const
{
	std::stringstream in;
	unsigned index = 0;
	in << "(if ";
	in << getCondition(index)->toString();
	in << " ";
	in << getThenBlock(index)->toString();

	for (index = 1; index < this->getIfNumber(); index++)
	{
		in << "(elseif ";
		in << getCondition(index)->toString();
		in << " ";
		in << getThenBlock(index)->toString();
	}

	if (getElseBlock() != nullptr)
		in << " else" << getElseBlock()->toString();
	in << ")";

	return in.str();
}

NS_STONE_END

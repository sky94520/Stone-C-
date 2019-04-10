#ifndef __Stone_IfStmnt_H__
#define __Stone_IfStmnt_H__

#include <vector>
#include <string>
#include <sstream>

#include "ASTList.h"

NS_STONE_BEGIN

class IfStmnt : public ASTList
{
private:
	ASTree* _elseBlock;
public:
	IfStmnt(const std::vector<ASTree*>& list, ASTree* elseBlock);

	ASTree* getCondition(unsigned int i) const;
	ASTree* getThenBlock(unsigned int i) const;
	ASTree* getElseBlock() const;
	unsigned int getIfNumber() const;

	virtual std::string toString() const;
};
NS_STONE_END
#endif
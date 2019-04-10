#ifndef __Stone_IfStmnt_H__
#define __Stone_IfStmnt_H__

#include <vector>
#include <string>
#include <sstream>

#include "ASTList.h"

NS_STONE_BEGIN

class IfStmnt : public ASTList
{
public:
	IfStmnt(const std::vector<ASTree*>& list);

	ASTree* getCondition() const;
	ASTree* getThenBlock() const;
	ASTree* getElseBlock() const;

	virtual std::string toString() const;
};
NS_STONE_END
#endif
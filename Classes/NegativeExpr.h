#ifndef __NegativeExpr_H__
#define __NegativeExpr_H__

#include <vector>
#include <string>

#include "ASTList.h"

NS_STONE_BEGIN

class ASTree;

class NegativeExpr : public ASTList
{
public:
	NegativeExpr(ASTree* a);
	NegativeExpr(const std::vector<ASTree*>& list);

	//?
	ASTree* getOperand();
	std::string toString() const;
};
NS_STONE_END
#endif
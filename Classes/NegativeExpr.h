#ifndef __Stone_NegativeExpr_H__
#define __Stone_NegativeExpr_H__

#include <vector>
#include <string>

#include "ASTList.h"

NS_STONE_BEGIN

class ASTree;
class Visitor;
class Environment;

class NegativeExpr : public ASTList
{
public:
	NegativeExpr(ASTree* a);
	NegativeExpr(const std::vector<ASTree*>& list);

	//?
	ASTree* getOperand();
	std::string toString() const;
public:
	virtual void accept(Visitor* v, Environment* env);
};
NS_STONE_END
#endif
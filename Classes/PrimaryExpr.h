#ifndef __Stone_PrimaryExpr_H__
#define __Stone_PrimaryExpr_H__

#include <vector>

#include "ASTList.h"

NS_STONE_BEGIN

class Visitor;
class Environment;

class PrimaryExpr : public ASTList
{
public:
	PrimaryExpr(const std::vector<ASTree*>& list);
	virtual ~PrimaryExpr();
public:
	virtual void accept(Visitor* v, Environment* env);

};
NS_STONE_END
#endif
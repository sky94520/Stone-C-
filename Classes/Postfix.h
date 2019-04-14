#ifndef __Stone_Postfix_H__
#define __Stone_Postfix_H__

#include <vector>

#include "ASTList.h"

NS_STONE_BEGIN

class Visitor;
class Environment;

class Postfix : public ASTList
{
public:
	Postfix();
	Postfix(const std::vector<ASTree*>& list);
	virtual ~Postfix();
public:
	virtual void accept(Visitor* v, Environment* env);
};
NS_STONE_END
#endif
#ifndef __Stone_ArrayLiteral_H__
#define __Stone_ArrayLiteral_H__

#include "ASTList.h"

NS_STONE_BEGIN

class Visitor;
class Environment;

/*
	用于数组的声明
*/
class ArrayLiteral : public ASTList
{
public:
	ArrayLiteral();
	ArrayLiteral(const std::vector<ASTree*>& list);
public:
	virtual void accept(Visitor* v, Environment* env);
};
NS_STONE_END
#endif
#ifndef __Stone_NumberLiteral_H__
#define __Stone_NumberLiteral_H__

#include "ASTLeaf.h"
NS_STONE_BEGIN

class Token;
class Visitor;
class Environment;

class NumberLiteral : public ASTLeaf
{
public:
	NumberLiteral(Token* token);
	//ªÒ»°÷µ
	int getValue() const;
public:
	virtual void accept(Visitor* v, Environment* env);
};
NS_STONE_END
#endif
#ifndef __Stone_StringLiteral_H__
#define __Stone_StringLiteral_H__

#include <string>

#include "ASTLeaf.h"

NS_STONE_BEGIN

class Token;
class Visitor;
class Environment;

class StringLiteral : public ASTLeaf
{
public:
	StringLiteral(Token* token);
	std::string getValue() const;
public:
	virtual void accept(Visitor* v, Environment* env);
};

NS_STONE_END
#endif
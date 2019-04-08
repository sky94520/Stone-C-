#ifndef __StringLiteral_H__
#define __StringLiteral_H__

#include <string>

#include "ASTLeaf.h"

NS_STONE_BEGIN

class Token;

class StringLiteral : public ASTLeaf
{
public:
	StringLiteral(Token* token);
	std::string getValue() const;
};

NS_STONE_END
#endif
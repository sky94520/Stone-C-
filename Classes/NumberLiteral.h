#ifndef __Stone_NumberLiteral_H__
#define __Stone_NumberLiteral_H__

#include "ASTLeaf.h"
NS_STONE_BEGIN

class Token;

class NumberLiteral : public ASTLeaf
{
public:
	NumberLiteral(Token* token);
	//ªÒ»°÷µ
	int getValue() const;
};
NS_STONE_END
#endif
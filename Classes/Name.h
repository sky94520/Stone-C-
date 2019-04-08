#ifndef __Name_H__
#define __Name_H__

#include "ASTLeaf.h"

NS_STONE_BEGIN

class Token;

class Name : public ASTLeaf
{
public:
	Name(Token* token);

	std::string getName() const;
};

NS_STONE_END
#endif
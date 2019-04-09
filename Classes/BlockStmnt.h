#ifndef __BlockStmnt_H__
#define __BlockStmnt_H__

#include <vector>
#include <string>

#include "ASTList.h"

NS_STONE_BEGIN

class BlockStmnt : public ASTList
{
public:
	BlockStmnt(const std::vector<ASTree*>& list);
};
NS_STONE_END
#endif
#include "BlockStmnt.h"
#include "ASTree.h"

NS_STONE_BEGIN

BlockStmnt::BlockStmnt(const std::vector<ASTree*>& list)
	:ASTList(list)
{
}
NS_STONE_END
#ifndef __NullStmnt_H__
#define __NullStmnt_H__

#include "ASTList.h"

NS_STONE_BEGIN

class NullStmnt : public ASTList
{
public:
	NullStmnt()
	{}
};

NS_STONE_END
#endif
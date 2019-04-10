#ifndef __Stone_WhileStmnt_H__
#define __Stone_WhileStmnt_H__

#include "ASTList.h"

NS_STONE_BEGIN

class WhileStmnt : public ASTList
{
public:
	WhileStmnt(const std::vector<ASTree*>& list);

	ASTree* getCondition() const;
	ASTree* getBody() const;

	virtual std::string toString() const;
};
NS_STONE_END
#endif
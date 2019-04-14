#ifndef __Stone_ClosureStmnt_H__
#define __Stone_ClosureStmnt_H__

#include "ASTList.h"

NS_STONE_BEGIN

class ParameterList;
class BlockStmnt;
class Visitor;
class Environment;

class ClosureStmnt : public ASTList
{
public:
	ClosureStmnt(const std::vector<ASTree*>& list);
	virtual ~ClosureStmnt();

	//获取函数参数列表
	ParameterList* getParameters() const;
	//获取函数体
	BlockStmnt* getBody() const;
public:
	virtual void accept(Visitor* v, Environment* env);
	virtual std::string toString() const;
};
NS_STONE_END
#endif
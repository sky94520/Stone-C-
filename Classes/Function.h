#ifndef __Stone_Function_H__
#define __Stone_Function_H__

#include "StObject.h"

NS_STONE_BEGIN

class ParameterList;
class BlockStmnt;
class Environment;

class Function: public Object
{
public:
	Function(ParameterList* parameters, BlockStmnt* block, Environment* env);
	~Function();

	ParameterList* getParameters() const;
	BlockStmnt* getBody() const;
	Environment* getEnvironment() const;
	//基于本环境创建环境
	Environment* makeEnv();
protected:
	ParameterList* _parameters;
	BlockStmnt* _body;
	Environment* _env;
};
NS_STONE_END
#endif // !__Stone_Function_H__

#include "Function.h"
#include "Environment.h"
#include "NestedEnv.h"
#include "ParameterList.h"
#include "BlockStmnt.h"

NS_STONE_BEGIN

Function::Function(ParameterList* parameters, BlockStmnt* block, Environment* env)
	:_parameters(parameters)
	,_body(block)
	,_env(env)
{
	//引用该环境
	_env->retain();

	_parameters->retain();
	_body->retain();
}

Function::~Function()
{
	_env->release();

	_parameters->release();
	_body->release();
}

ParameterList* Function::getParameters() const
{
	return _parameters;
}

BlockStmnt* Function::getBody() const
{
	return _body;
}

Environment* Function::getEnvironment() const
{
	return _env;
}

Environment* Function::makeEnv()
{
	return new NestedEnv(_env);
}
NS_STONE_END
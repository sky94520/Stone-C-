#include "Function.h"
#include "Environment.h"
#include "NestedEnv.h"

NS_STONE_BEGIN
Function::Function(Environment* env)
	:_env(env)
{
	//引用该环境
	_env->retain();
}

Function::~Function()
{
	_env->release();
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
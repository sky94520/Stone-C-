#include "Function.h"
#include "Environment.h"
#include "NestedEnv.h"

NS_STONE_BEGIN
Function::Function(Environment* env)
	:_env(env)
{
	//���øû���
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
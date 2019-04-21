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
	//����һ���Զ��ͷŵĻ���
	auto env = new NestedEnv(_env);
	env->autorelease();
	return env;
}

NS_STONE_END
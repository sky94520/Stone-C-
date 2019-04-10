#include "NestedEnv.h"

NS_STONE_BEGIN

NestedEnv::NestedEnv()
	:NestedEnv(nullptr)
{
}

NestedEnv::NestedEnv(Environment* env)
{
	if (env != nullptr)
	{
		_outer = env;
		_outer->retain();
	}
}

NestedEnv::~NestedEnv()
{
	if (_outer != nullptr)
		_outer->release();

	//清除变量
	_values.clear();
}

void NestedEnv::setOuter(Environment* env)
{
	env->retain();
	if (_outer != nullptr)
	{
		_outer->release();
	}
	_outer = env;
}
	
void NestedEnv::putNew(const std::string& name, const Value& value)
{
	_values.emplace(name, value);
}

Environment* NestedEnv::where(const std::string& name)
{
	//本环境下存在该变量，则直接返回
	if (_values.find(name) != _values.end())
		return this;
	else if (_outer == nullptr)
		return nullptr;
	else
		return static_cast<NestedEnv*>(_outer)->where(name);
}
void NestedEnv::put(const std::string& name, const Value& value)
{
	//获取该变量名所处的环境
	Environment* env = this->where(name);

	if (env == nullptr)
		env = this;

	env->put(name, value);
}

const Value& NestedEnv::get(const std::string& name) const
{
	//先在本环境中查找
	auto it = _values.find(name);

	if (it == _values.end() && _outer != nullptr)
		return _outer->get(name);
	else
	{
		auto& value = it->second;
		return value;
	}
}
NS_STONE_END
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

	//�������
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

Environment* NestedEnv::where(const std::string& name)
{
	//�������´��ڸñ�������ֱ�ӷ���
	if (_values.find(name) != _values.end())
		return this;
	else if (_outer == nullptr)
		return nullptr;
	else
		return static_cast<NestedEnv*>(_outer)->where(name);
}
	
void NestedEnv::putNew(const std::string& name, const Value& value)
{
	auto it = _values.find(name);

	if (it != _values.end())
	{
		it->second = value;
	}
	else
		_values.emplace(name, value);
}

void NestedEnv::put(const std::string& name, const Value& value)
{
	//��ȡ�ñ����������Ļ���
	Environment* env = this->where(name);

	if (env == nullptr)
		env = this;

	static_cast<NestedEnv*>(env)->putNew(name, value);
}

const Value* NestedEnv::get(const std::string& name) const
{
	//���ڱ������в���
	auto it = _values.find(name);

	//�ڱ������ڴ��ڸñ���
	if (it != _values.end())
	{
		return &(it->second);
	}
	//�����������ڸñ������и�����
	else if (_outer != nullptr)
	{
		return _outer->get(name);
	}
	return nullptr;
}

Value* NestedEnv::get(const std::string& name)
{
	//���ڱ������в���
	auto it = _values.find(name);

	//�ڱ������ڴ��ڸñ���
	if (it != _values.end())
	{
		return &(it->second);
	}
	//�����������ڸñ������и�����
	else if (_outer != nullptr)
	{
		return _outer->get(name);
	}
	return nullptr;
}
NS_STONE_END
#ifndef __Stone_Environment_H__
#define __Stone_Environment_H__

#include <string>

#include "STObject.h"
#include "Value.h"
#include "NativeFunction.h"

NS_STONE_BEGIN

/*���������ڱ���ű��ļ�ֵ��*/
class Environment: public Object
{
public:
	virtual ~Environment() {}

	//�ڱ�������ֱ�Ӳ���
	virtual void putNew(const std::string& name, const Value& value) = 0;

	//�������
	virtual void put(const std::string& name, const Value& value) = 0;
	//��ȡ����
	virtual const Value* get(const std::string& name) const = 0;
	virtual Value* get(const std::string& name) = 0;
public:
	//�ڱ�������ֱ�Ӳ��뺯��
	void putNative(const std::string& name, const nativeFunc& callback, const char** params, int len)
	{
		NativeFunction* function = new NativeFunction(params, len, callback, this);
		this->putNew(name, Value(function));
		function->release();
	}
};
NS_STONE_END
#endif
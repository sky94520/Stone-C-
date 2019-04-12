#ifndef __Stone_Environment_H__
#define __Stone_Environment_H__

#include <string>

#include "StoneMarcos.h"
#include "Value.h"

NS_STONE_BEGIN

class Environment
{
protected:
	Environment()
		:_referenceCount(0)
	{}
public:
	virtual ~Environment() {}
	virtual void put(const std::string& name, const Value& value) = 0;
	virtual const Value* get(const std::string& name) const = 0;

	//�������
	void retain()
	{
		_referenceCount++;
	}
	//�ͷ�����
	void release()
	{
		_referenceCount--;

		if (_referenceCount == 0)
			delete this;
	}
private:
	//���ü���
	unsigned int _referenceCount;
};
NS_STONE_END
#endif
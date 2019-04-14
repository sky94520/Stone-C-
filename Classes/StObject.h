#ifndef __Stone_Object_H__
#define __Stone_Object_H__

#include "StoneMarcos.h"

NS_STONE_BEGIN

class Object
{
public:
	Object()
		:_referenceCount(1)
	{}
	virtual ~Object() {}
	//添加引用
	void retain()
	{
		_referenceCount++;
	}
	//释放引用
	void release()
	{
		_referenceCount--;

		if (_referenceCount == 0)
			delete this;
	}
protected:
	//引用计数
	unsigned int _referenceCount;
};
NS_STONE_END
#endif
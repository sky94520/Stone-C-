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
protected:
	//���ü���
	unsigned int _referenceCount;
};
NS_STONE_END
#endif
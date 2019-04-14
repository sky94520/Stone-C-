#ifndef __Stone_Object_H__
#define __Stone_Object_H__

#include "StoneMarcos.h"

NS_STONE_BEGIN

class AutoreleasePool;

//所有类的基类
class Object
{
protected:
	//引用计数器
	unsigned int _referenceCount;
	//是否由内存管理器管理
	bool _managed;
public:
	bool init();
	Object();
	virtual ~Object();
	//保留
	void retain();
	//释放
	void release();
	//自动释放
	Object*autorelease();
	//获得引用数量
	unsigned int getReferenceCount() const { return _referenceCount; }
	//是否交给释放池
	bool isManaged() const { return _managed; }
	//友元
	friend class AutoreleasePool;
};
NS_STONE_END
#endif
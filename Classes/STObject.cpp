#include "STObject.h"
#include "STAutoreleasePool.h"

NS_STONE_BEGIN
Object::Object()
	:_referenceCount(1)
	,_managed(false)
{
}

Object::~Object() 
{
	//被自动释放管理器管理，调用内存管理器删除自己
	if (_managed)
	{
		AutoreleasePool::getInstance()->removeObject(this);
	}
}
//添加引用
void Object::retain()
{
	_referenceCount++;
}
//释放引用
void Object::release()
{
	_referenceCount--;

	if (_referenceCount == 0)
		delete this;
}

Object* Object::autorelease()
{
	//不重复插入
	if (_managed == false)
	{
		//加入自动释放池中
		AutoreleasePool::getInstance()->addObject(this);
		_managed = true;
	}
	return this;
}

NS_STONE_END
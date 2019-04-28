#include "STObject.h"
#include "STAutoreleasePool.h"

NS_STONE_BEGIN
Object::Object()
	:_referenceCount(1)
	,_managed(false)
	,_lock(false)
{
}

Object::~Object() 
{
	//���Զ��ͷŹ��������������ڴ������ɾ���Լ�
	if (_managed)
	{
		AutoreleasePool::getInstance()->removeObject(this);
	}
}

void Object::retain()
{
	//δ�����Ż��������
	if (!_lock)
	{
		_referenceCount++;
	}
}

void Object::release(bool force)
{
	//�����ҷ�ǿ�ƣ����˳�
	if (_lock && !force)
		return;

	_referenceCount--;

	if (_referenceCount == 0)
		delete this;
}

Object* Object::autorelease()
{
	//���ظ�����
	if (_managed == false)
	{
		//�����Զ��ͷų���
		AutoreleasePool::getInstance()->addObject(this);
		_managed = true;
	}
	return this;
}

NS_STONE_END
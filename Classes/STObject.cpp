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
	//���Զ��ͷŹ��������������ڴ������ɾ���Լ�
	if (_managed)
	{
		AutoreleasePool::getInstance()->removeObject(this);
	}
}
//�������
void Object::retain()
{
	_referenceCount++;
}
//�ͷ�����
void Object::release()
{
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
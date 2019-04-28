#ifndef __Stone_Object_H__
#define __Stone_Object_H__

#include "StoneMarcos.h"

NS_STONE_BEGIN

class AutoreleasePool;

//������Ļ���
class Object
{
public:
	bool init();
	Object();
	virtual ~Object();
	//����
	void retain();
	//�ͷ�
	void release(bool force = false);
	//�Զ��ͷ�
	Object*autorelease();
	//�����������
	unsigned int getReferenceCount() const { return _referenceCount; }
	//�Ƿ񽻸��ͷų�
	bool isManaged() const { return _managed; }

	bool isLock() const { return _lock; }
	void setLock(bool lock) { _lock = lock; }
	//��Ԫ
	friend class AutoreleasePool;
protected:
	//���ü�����
	unsigned int _referenceCount;
	//�Ƿ����ڴ����������
	bool _managed;
	//���� �����������retain release����ı��������������������������ǻ��ͷ�
	bool _lock;
};
NS_STONE_END
#endif
#ifndef __Stone_Object_H__
#define __Stone_Object_H__

#include "StoneMarcos.h"

NS_STONE_BEGIN

class AutoreleasePool;

//������Ļ���
class Object
{
protected:
	//���ü�����
	unsigned int _referenceCount;
	//�Ƿ����ڴ����������
	bool _managed;
public:
	bool init();
	Object();
	virtual ~Object();
	//����
	void retain();
	//�ͷ�
	void release();
	//�Զ��ͷ�
	Object*autorelease();
	//�����������
	unsigned int getReferenceCount() const { return _referenceCount; }
	//�Ƿ񽻸��ͷų�
	bool isManaged() const { return _managed; }
	//��Ԫ
	friend class AutoreleasePool;
};
NS_STONE_END
#endif
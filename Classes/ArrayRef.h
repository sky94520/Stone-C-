#ifndef __Stone_ArrayRef_H__
#define __Stone_ArrayRef_H__

#include <string>
#include <vector>

#include "ASTList.h"

NS_STONE_BEGIN

class Visitor;
class Environment;

/*
	���������ʹ�ã�����������Ӧ��Ԫ��
*/
class ArrayRef : public ASTList
{
public:
	static const std::string TREE_ID;
public:
	ArrayRef(ASTree* t);
	ArrayRef(const std::vector<ASTree*>& list);
	ASTree* getIndex() const;
public:
	virtual void accept(Visitor* v, Environment* env);
	virtual std::string toString() const;
};
NS_STONE_END
#endif
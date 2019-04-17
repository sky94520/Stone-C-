#ifndef __Stone_DefStmnt_H__
#define __Stone_DefStmnt_H__

#include <vector>
#include <string>
#include <sstream>

#include "ASTList.h"

NS_STONE_BEGIN

class ParameterList;
class BlockStmnt;
class Visitor;
class Environment;

class DefStmnt : public ASTList
{
public:
	static const std::string TREE_ID;
public:
	DefStmnt(const std::vector<ASTree*>& list);
	virtual ~DefStmnt();

	//��ȡ��������
	std::string getName() const;
	//��ȡ���������б�
	ParameterList* getParameters() const;
	//��ȡ������
	BlockStmnt* getBody() const;
public:
	virtual void accept(Visitor* v, Environment* env);
	virtual std::string toString() const;
};
NS_STONE_END
#endif

#ifndef __Stone_Function_H__
#define __Stone_Function_H__

#include <string>

#include "StObject.h"

NS_STONE_BEGIN

class ParameterList;
class BlockStmnt;
class Environment;
class Visitor;

class Function: public Object
{
public:
	Function(Environment* env);
	~Function();
	Environment* getEnvironment() const;
	//���ڱ�������������
	Environment* makeEnv();
public:
	//��ȡ��������
	virtual unsigned int getParamSize() const = 0;
	//��ȡ������
	virtual std::string getParamName(unsigned index) const = 0;
	//ִ�к���
	virtual void execute(Visitor* v, Environment* env) = 0;
protected:
	Environment* _env;
};
NS_STONE_END
#endif // !__Stone_Function_H__

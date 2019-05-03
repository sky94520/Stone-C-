#ifndef __Stone_EvalVisitor_H__
#define __Stone_EvalVisitor_H__

#include <string>

#include "Visitor.h"
#include "Value.h"

NS_STONE_BEGIN

class Environment;
class ASTree;
class ASTList;
class ASTLeaf;
class NumberLiteral;
class StringLiteral;
class Name;
class NegativeExpr;
class BinaryExpr;
class BlockStmnt;
class IfStmnt;
class WhileStmnt;

class PrimaryExpr;
class Postfix;
class Arguments;
class ParameterList;
class DefStmnt;
class ClosureStmnt;

/*ִ�г����﷨��*/
class EvalVisitor : public Visitor 
{
public:
	EvalVisitor();
	virtual ~EvalVisitor();
public:
	//������������������õ�
	virtual void visit(ASTree* t, Environment* env);
	virtual void visit(ASTList* t, Environment* env);
	virtual void visit(ASTLeaf* t, Environment* env);

	//��ȡ���ڰ�������ֵ��������result��
	virtual void visit(NumberLiteral* t, Environment* env);
	//��ȡ���ڰ������ַ�����������result��
	virtual void visit(StringLiteral* t, Environment* env);
	//��ȡ��Ӧ���ֵ�Value��������result��
	virtual void visit(Name* t, Environment* env);
	//Ŀǰ�����ڶ����͵�ת��
	virtual void visit(NegativeExpr* t, Environment* env);
	//������ʽ�������ֵ����result��
	virtual void visit(BinaryExpr* t, Environment* env);
	//���п���룬���һ������ֵ����result��
	virtual void visit(BlockStmnt* t, Environment* env);
	//������������,���һ������ֵ����result��
	virtual void visit(IfStmnt* t, Environment* env);
	//����ѭ�����飬���һ������ֵ����result��
	virtual void visit(WhileStmnt* t, Environment* env);

	//----------------�������-----------------
	//����ִ��
	virtual void visit(PrimaryExpr* t, Environment* env);
	//һ�㲻��ִ��
	virtual void visit(Postfix* t, Environment* env);
	//�����ĺ���ִ��
	virtual void visit(Arguments* t, Environment* env);
	//�������壬����Ӻ����������У����Ѻ���������result��
	virtual void visit(DefStmnt* t, Environment* env);

	virtual void visit(ClosureStmnt* t, Environment* env);

	//----------------�������-----------------
	//����
	virtual void visit(ArrayLiteral* t, Environment* env);
	virtual void visit(ArrayRef* t, Environment* env);
public:
	void setResult(int value);
	void setResult(const std::string& value);
	void setResult(const std::vector<Value>& value);
	void setResult(Function* value);
	void setResult(const Value& value);
	void setResult(Value* value);
private:
	//------BinaryExpr----
	void computeOp(BinaryExpr* binaryExpr, const std::string& op, Environment* env);
	void computeAssign(BinaryExpr* binaryExpr, Environment* env);
	//TODO: ��ʱΪ����
	int computeNumber(ASTree* t, int left, const std::string& op, int right);

	//------PrimaryExpr-----
	void evalSubExpr(PrimaryExpr* t, Environment* env, int nest);

public:
	Value* result;
private:
	bool _allocated;
};
NS_STONE_END
#endif // ! __Stone_EvalVisitor_H__

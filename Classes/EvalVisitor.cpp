#include "EvalVisitor.h"
#include "Token.h"
#include "Environment.h"
#include "StoneException.h"
#include "ASTree.h"
#include "ASTList.h"
#include "ASTLeaf.h"
#include "NumberLiteral.h"
#include "StringLiteral.h"
#include "Name.h"
#include "NegativeExpr.h"
#include "BinaryExpr.h"
#include "BlockStmnt.h"
#include "IfStmnt.h"
#include "WhileStmnt.h"

NS_STONE_BEGIN
void EvalVisitor::visit(ASTree* t, Environment* env)
{
}

void EvalVisitor::visit(ASTList* t, Environment* env)
{
}

void EvalVisitor::visit(ASTLeaf* t, Environment* env)
{
}

void EvalVisitor::visit(NumberLiteral* t, Environment* env)
{
	result = t->getToken()->asInt();
}

void EvalVisitor::visit(StringLiteral* t, Environment* env)
{
	result = t->getToken()->asString();
}

void EvalVisitor::visit(Name* t, Environment* env)
{
	//��ȡ����������
	auto name = t->getName();
	//��ȡ������Ӧ��ֵ
	const Value* value = env->get(name);
	if (value == nullptr)
	{
		throw StoneException("undefined name: " + name, t);
	}
	else
		result = Value(*value);
}

void EvalVisitor::visit(NegativeExpr* t, Environment* env)
{
}

void EvalVisitor::visit(BinaryExpr* t, Environment* env)
{
	//��ȡ������
	std::string op = t->getOperator();
	//��ֵ���
	if (op == "=")
	{
		//������ֵ
		t->getRight()->accept(this, env);
		//�ݴ�ֵ
		Value right = this->result;
		//��ֵ������Name,�����޸ĵ���ֵ
		Name* left = static_cast<Name*>(t->getLeft());
		if (left == nullptr)
			throw StoneException("bad assignment", t);
	}
	else
	{
		t->getLeft()->accept(this, env);
		Value left = this->result;

		t->getRight()->accept(this, env);
		Value right = this->result;

		//TODO:��ʱ����
	}
}

void EvalVisitor::visit(BlockStmnt* t, Environment* env)
{

}

void EvalVisitor::visit(IfStmnt* t, Environment* env)
{

}

void EvalVisitor::visit(WhileStmnt* t, Environment* env)
{

}
NS_STONE_END

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
	//获取变量的名称
	auto name = t->getName();
	//获取变量对应的值
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
	//获取操作符
	std::string op = t->getOperator();
	//赋值语句
	if (op == "=")
	{
		//计算右值
		t->getRight()->accept(this, env);
		//暂存值
		Value right = this->result;
		//左值必须是Name,即可修改的左值
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

		//TODO:暂时返回
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

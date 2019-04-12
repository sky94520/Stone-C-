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
	t->getOperand()->accept(this, env);
	//TODO:只有数字才能使用负号
	if (this->result.getType() == Value::Type::INTEGER)
	{
		this->result = Value(-this->result.asInt());
	}
	else
		throw StoneException("bad type for -", t);
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
		Name* left = dynamic_cast<Name*>(t->getLeft());
		if (left == nullptr)
			throw StoneException("bad assignment", t);
		//添加到环境中
		env->put(left->getName(), this->result);
	}
	else
	{
		t->getLeft()->accept(this, env);
		Value left = this->result;

		t->getRight()->accept(this, env);
		Value right = this->result;
		//计算值
		Value value = this->computeOp(t, left, op, right);
		//暂存值
		this->result = value;
	}
}

void EvalVisitor::visit(BlockStmnt* t, Environment* env)
{
	//遍历节点，并调用accept
	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = *it;
		child->accept(this, env);
	}
}

void EvalVisitor::visit(IfStmnt* t, Environment* env)
{
	//获取当前的条件语句
	unsigned int size = t->getIfNumber();

	//遍历if {elseif}
	for (unsigned int i = 0; i < size; i++)
	{
		t->getCondition(i)->accept(this, env);
		//判断返回值
		if (this->result.asBool())
		{
			t->getThenBlock(i);
			return ;
		}
	}
	//以上不满足条件，则尝试执行else块
	if (t->getElseBlock() != nullptr)
	{
		t->getElseBlock()->accept(this, env);
	}
}

void EvalVisitor::visit(WhileStmnt* t, Environment* env)
{
	do 
	{
		//判断条件
		t->getCondition()->accept(this, env);
		if (!this->result.asBool())
			break;
		//执行语句
		t->getBody()->accept(this, env);

	} while (1);
}

Value EvalVisitor::computeOp(ASTree* t, const Value& left, const std::string& op, const Value& right)
{
	Value value;
	if (left.getType() == Value::Type::INTEGER && right.getType() == Value::Type::INTEGER)
	{
		value = this->computeNumber(t, left.asInt(), op, right.asInt());
	}
	//转换为字符串
	else if ("+" == op)
	{
		value = Value(left.asString() + right.asString());
	}
	else if ("==" == op)
	{
		value = Value(left == right ? true : false);
	}
	else
		throw StoneException("bad type", t);

	return value;
}

int EvalVisitor::computeNumber(ASTree* t, int left, const std::string& op, int right)
{
	if ("+" == op)
		return left + right;
	else if ("-" == op)
		return left - right;
	else if ("*" == op)
		return left * right;
	else if ("/" == op)
		return left / right;
	else if ("%" == op)
		return left % right;
	else if ("==" == op)
		return left == right;
	else if (">" == op)
		return left > right;
	else if ("<" == op)
		return left < right;
	else
		throw StoneException("bad operator", t);
}
NS_STONE_END

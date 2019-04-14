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
#include "PrimaryExpr.h"
#include "Postfix.h"
#include "Arguments.h"
#include "ParameterList.h"
#include "DefStmnt.h"
#include "Function.h"

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
	{
		result = Value(*value);
	}
}

void EvalVisitor::visit(NegativeExpr* t, Environment* env)
{
	t->getOperand()->accept(this, env);
	//TODO:ֻ�����ֲ���ʹ�ø���
	if (this->result.getType() == Value::Type::INTEGER)
	{
		this->result = Value(-this->result.asInt());
	}
	else
		throw StoneException("bad type for -", t);
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
		Name* left = dynamic_cast<Name*>(t->getLeft());
		if (left == nullptr)
			throw StoneException("bad assignment", t);
		//���ӵ�������
		env->put(left->getName(), this->result);
	}
	else
	{
		t->getLeft()->accept(this, env);
		Value left = this->result;

		t->getRight()->accept(this, env);
		Value right = this->result;
		//����ֵ
		Value value = this->computeOp(t, left, op, right);
		//�ݴ�ֵ
		this->result = value;
	}
}

void EvalVisitor::visit(BlockStmnt* t, Environment* env)
{
	//�����ڵ㣬������accept
	for (auto it = t->begin(); it != t->end(); it++)
	{
		ASTree* child = *it;
		child->accept(this, env);
	}
}

void EvalVisitor::visit(IfStmnt* t, Environment* env)
{
	//��ȡ��ǰ���������
	unsigned int size = t->getIfNumber();

	//����if {elseif}
	for (unsigned int i = 0; i < size; i++)
	{
		t->getCondition(i)->accept(this, env);
		//�жϷ���ֵ
		if (this->result.asBool())
		{
			t->getThenBlock(i)->accept(this, env);
			return ;
		}
	}
	//���ϲ���������������ִ��else��
	if (t->getElseBlock() != nullptr)
	{
		t->getElseBlock()->accept(this, env);
	}
}

void EvalVisitor::visit(WhileStmnt* t, Environment* env)
{
	Value value;
	do 
	{
		//�ж�����
		t->getCondition()->accept(this, env);
		if (!this->result.asBool())
			break;
		//ִ�����
		t->getBody()->accept(this, env);
		//�ݴ淵��ֵ
		value = this->result;

	} while (1);
	this->result = value;
}

void EvalVisitor::visit(PrimaryExpr* t, Environment* env)
{
	//Name {Arguments}
	this->evalSubExpr(t, env, 0);
}

void EvalVisitor::visit(Postfix* t, Environment* env)
{
}

void EvalVisitor::visit(Arguments* t, Environment* env)
{
	Function* function = this->result.asFunction();
	//��ȡfunction��Ҫ�Ĳ����б�
	ParameterList* params = function->getParameters();

	if (t->getSize() != params->getSize())
		throw StoneException("bad number of arguments", t);
	//����һ���µĻ���
	Environment* newEnv = function->makeEnv();
	//��������Ͷ�Ӧ��ֵ
	for (int i = 0; i < t->getNumChildren(); i++)
	{
		auto args = t->getChild(i);
		//�ȼ���
		args->accept(this, env);
		//����
		this->visit(params, newEnv, i);
	}
	//ִ�к�����
	this->visit(function->getBody(), newEnv);

	newEnv->release();
}

void EvalVisitor::visit(ParameterList* t, Environment* env, unsigned index)
{
	env->putNew(t->getName(index), this->result);
}

void EvalVisitor::visit(DefStmnt* t, Environment* env)
{
	//ֱ���ڱ�����������Function����
	Function* function = new Function(t->getParameters(), t->getBody(), env);
	Value value = Value(function);
	env->putNew(t->getName(), value);
	this->result = t->getName();

	function->release();
}

//---------------------------------BinaryExpr---------------------------
Value EvalVisitor::computeOp(ASTree* t, const Value& left, const std::string& op, const Value& right)
{
	Value value;
	if (left.getType() == Value::Type::INTEGER && right.getType() == Value::Type::INTEGER)
	{
		value = this->computeNumber(t, left.asInt(), op, right.asInt());
	}
	//ת��Ϊ�ַ���
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
//---------------------------PrimaryExpr---------------------
void EvalVisitor::evalSubExpr(PrimaryExpr* t, Environment* env, int nest)
{
	//�������� foo(2)(3) ���δ������ҵ���
	if (t->getNumChildren() - nest > 1)
	{
		this->evalSubExpr(t, env, nest + 1);
		//TODO:Ŀǰ��ǿ��ת��ΪArguments
		auto postfix = t->getChild(t->getNumChildren() - nest - 1);
		//����Arguments,�����ú��� ����ĺ����Ѿ���this->result֮��
		postfix->accept(this, env);
	}
	//�������ֶ�Ӧ�ĺ�����������this->result֮��
	else
	{
		auto name = static_cast<Name*>(t->getChild(0));
		this->visit(name, env);
	}
}
NS_STONE_END